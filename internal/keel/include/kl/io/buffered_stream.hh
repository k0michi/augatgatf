#ifndef KL_IO_PATH_HH
#define KL_IO_PATH_HH

#include <cstring>

#include "kl/io/async_stream.hh"

namespace kl::io {
class BufferedReader final {
public:
  static constexpr std::size_t kDefaultBufferSize = 8192;

  template <AsyncReadable T>
  explicit BufferedReader(T &readable,
                          std::size_t bufferSize = kDefaultBufferSize)
      : readable_(std::make_unique<Wrapper<T>>(readable)), buffer_(bufferSize),
        head_(0), tail_(0) {}

  kl::concurrent::Task<Expected<std::size_t>>
  read(std::shared_ptr<Loop> loop, std::span<std::byte> buffer) {
    std::size_t totalRead = 0;

    while (totalRead < buffer.size()) {
      if (head_ == tail_) {
        // Buffer is empty
        head_ = 0;
        tail_ = 0;
        auto nread = co_await readable_->read(
            loop, std::span{buffer_.data(), buffer_.size()});
        if (!nread) {
          co_return std::unexpected(nread.error());
        }
        if (*nread == 0) {
          // EOF
          break;
        }
        tail_ += *nread;
      }

      std::size_t toCopy = std::min(buffer.size() - totalRead, tail_ - head_);
      std::memcpy(buffer.data() + totalRead, buffer_.data() + head_, toCopy);
      head_ += toCopy;
      totalRead += toCopy;
    }

    co_return totalRead;
  }

private:
  template <AsyncReadable T> class Wrapper : public IAsyncReadable {
  public:
    explicit Wrapper(T &inner) : inner_(inner) {}

    kl::concurrent::Task<Expected<std::size_t>>
    read(std::shared_ptr<kl::io::Loop> loop,
         std::span<std::byte> buffer) override {
      co_return co_await inner_.read(loop, buffer);
    }

  private:
    T &inner_;
  };

  std::unique_ptr<IAsyncReadable> readable_;
  std::vector<std::byte> buffer_;
  std::size_t head_;
  std::size_t tail_;
};
} // namespace kl::io
#endif