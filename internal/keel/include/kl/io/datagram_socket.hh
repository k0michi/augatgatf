#ifndef KL_IO_DATAGRAM_SOCKET_HH
#define KL_IO_DATAGRAM_SOCKET_HH

#include <coroutine>
#include <cstring>
#include <type_traits>
#include <utility>
#include <vector>

#include <uv.h>

#include "addr.hh"
#include "io_exception.hh"
#include "kl/concurrent/task.hh"
#include "loop.hh"

namespace kl::io {
class ReceiveAwaiter {
public:
  ReceiveAwaiter(uv_udp_t *udpHandle) : udpHandle_(udpHandle) {}

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> handle) {
    udpHandle_->data = this;
    handle_ = handle;

    uv_udp_recv_start(
        udpHandle_,
        [](uv_handle_t *handle, std::size_t suggested_size, uv_buf_t *buf) {
          buf->base = static_cast<char *>(malloc(suggested_size));
          buf->len = suggested_size;
        },
        [](uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf,
           const struct sockaddr *addr, unsigned flags) {
          uv_udp_recv_stop(handle);
          auto self = static_cast<ReceiveAwaiter *>(handle->data);

          if (nread < 0) {
            self->expected_ = std::unexpected(IOException(uv_strerror(nread)));
          } else if (nread == 0 && addr == nullptr) {
            self->expected_ = std::make_pair(std::vector<std::byte>{}, Addr{});
          } else {
            std::vector<std::byte> data;
            data.resize(static_cast<std::size_t>(nread));
            std::memcpy(data.data(), buf->base,
                        static_cast<std::size_t>(nread));
            auto addrResult = Addr::fromPosixAddr(addr);

            if (addrResult.has_value()) {
              self->expected_ =
                  std::make_pair(std::move(data), addrResult.value());
            } else {
              self->expected_ = std::unexpected(addrResult.error());
            }
          }

          if (buf->base) {
            free(buf->base);
          }

          self->handle_.resume();
        });
  }

  Expected<std::pair<std::vector<std::byte>, Addr>> await_resume() {
    return expected_;
  }

private:
  uv_udp_t *udpHandle_;
  std::coroutine_handle<> handle_;
  Expected<std::pair<std::vector<std::byte>, Addr>> expected_;
};

class CloseAwaiter {
public:
  CloseAwaiter(uv_udp_t *udpHandle) : udpHandle_(udpHandle) {}

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> handle) {
    udpHandle_->data = handle.address();

    uv_close(reinterpret_cast<uv_handle_t *>(udpHandle_),
             [](uv_handle_t *handle) {
               auto h = std::coroutine_handle<>::from_address(handle->data);
               h.resume();
             });
  }

  Expected<void> await_resume() { return {}; }

private:
  uv_udp_t *udpHandle_;
};

enum class DatagramSocketFlag : unsigned int {
  eIPv6Only = UV_UDP_IPV6ONLY,
  eReuseAddr = UV_UDP_REUSEADDR,
  eRecvErr = UV_UDP_LINUX_RECVERR,
};

class DatagramSocket {
public:
  DatagramSocket() noexcept = default;

  DatagramSocket(const DatagramSocket &) = delete;
  DatagramSocket &operator=(const DatagramSocket &) = delete;

  DatagramSocket(DatagramSocket &&other) noexcept { swap(*this, other); }

  DatagramSocket &operator=(DatagramSocket &&other) noexcept {
    DatagramSocket temp(std::move(other));
    swap(*this, temp);
    return *this;
  }

  friend void swap(DatagramSocket &first, DatagramSocket &second) noexcept {
    using std::swap;
    swap(first.own_, second.own_);
    swap(first.udpHandle_, second.udpHandle_);
  }

public:
  static Expected<std::shared_ptr<DatagramSocket>>
  create(std::shared_ptr<Loop> loop) {
    auto datagram = std::shared_ptr<DatagramSocket>(new DatagramSocket());
    int result = uv_udp_init(loop->getUVLoop(), &datagram->udpHandle_);
    datagram->own_ = true;

    if (result < 0) {
      return std::unexpected(IOException(uv_strerror(result)));
    }

    return datagram;
  }

  Expected<void> bind(const Addr &addr, DatagramSocketFlag flags = {}) {
    auto posixAddr = addr.asPosixAddr();

    int result = uv_udp_bind(
        &udpHandle_, reinterpret_cast<const struct sockaddr *>(posixAddr),
        static_cast<unsigned int>(flags));

    if (result < 0) {
      return std::unexpected(IOException(uv_strerror(result)));
    }

    return {};
  }

  kl::concurrent::Task<Expected<std::pair<std::vector<std::byte>, Addr>>>
  receive() {
    while (true) {
      auto result = co_await ReceiveAwaiter(&udpHandle_);

      if (result.has_value() && result->first.size() > 0) {
        co_return result;
      } else if (!result.has_value()) {
        co_return result;
      }
    }
  }

  kl::concurrent::Task<Expected<void>> close() {
    co_return co_await CloseAwaiter(&udpHandle_);
  }

  ~DatagramSocket() noexcept {
    if (own_) {
      uv_close(reinterpret_cast<uv_handle_t *>(&udpHandle_), nullptr);
    }
  }

private:
  bool own_ = false;
  uv_udp_t udpHandle_;
};
} // namespace kl::io
#endif