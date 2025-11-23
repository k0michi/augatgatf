#ifndef KL_IO_AWAITER_HH
#define KL_IO_AWAITER_HH

#include <coroutine>
#include <string>

#include <uv.h>

#include "kl/io/io_exception.hh"

namespace kl::io {
struct OpenAwaiter {
  uv_loop_t *loop;
  std::string path;
  int flags;
  int mode;
  uv_file fd = -1;
  uv_fs_t req;

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> h) {
    req.data = h.address();
    uv_fs_open(loop, &req, path.c_str(), flags, mode, [](uv_fs_t *req) {
      auto handle = std::coroutine_handle<>::from_address(req->data);
      handle.resume();
    });
  }

  Expected<uv_file> await_resume() {
    if (req.result < 0) {
      uv_fs_req_cleanup(&req);
      return std::unexpected(IOException("uv_fs_open failed: " +
                                         std::string(uv_strerror(req.result))));
    }
    fd = static_cast<uv_file>(req.result);
    uv_fs_req_cleanup(&req);
    return fd;
  }
};

struct CloseAwaiter {
  uv_file fd;
  uv_loop_t *loop;
  uv_fs_t req;

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> h) {
    req.data = h.address();
    uv_fs_close(loop, &req, fd, [](uv_fs_t *req) {
      auto handle = std::coroutine_handle<>::from_address(req->data);
      handle.resume();
    });
  }

  Expected<void> await_resume() {
    if (req.result < 0) {
      uv_fs_req_cleanup(&req);
      return std::unexpected(IOException("uv_fs_close failed: " +
                                         std::string(uv_strerror(req.result))));
    }
    uv_fs_req_cleanup(&req);
    return {};
  }
};

struct ReadAwaiter {
  uv_file fd;
  uv_loop_t *loop;
  std::byte *buf;
  unsigned int len;
  ssize_t result;
  uv_fs_t req;
  std::int64_t offset;

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> h) {
    req.data = h.address();

    uv_buf_t uvbuf = uv_buf_init(reinterpret_cast<char *>(buf), len);

    uv_fs_read(loop, &req, fd, &uvbuf, 1, offset, [](uv_fs_t *req) {
      auto handle = std::coroutine_handle<>::from_address(req->data);
      handle.resume();
    });
  }

  Expected<std::size_t> await_resume() {
    if (req.result < 0) {
      return std::unexpected(IOException("uv_fs_read failed: " +
                                         std::string(uv_strerror(req.result))));
    }

    std::size_t nread = static_cast<std::size_t>(req.result);
    uv_fs_req_cleanup(&req);

    return nread;
  }
};

struct WriteAwaiter {
  uv_file fd;
  uv_loop_t *loop;
  const std::byte *buf;
  unsigned int len;
  ssize_t result;
  uv_fs_t req;
  std::int64_t offset;

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> h) {
    req.data = h.address();

    uv_buf_t uvbuf = uv_buf_init(
        const_cast<char *>(reinterpret_cast<const char *>(buf)), len);

    uv_fs_write(loop, &req, fd, &uvbuf, 1, offset, [](uv_fs_t *req) {
      auto handle = std::coroutine_handle<>::from_address(req->data);
      handle.resume();
    });
  }

  Expected<std::size_t> await_resume() {
    if (req.result < 0) {
      return std::unexpected(IOException("uv_fs_write failed: " +
                                         std::string(uv_strerror(req.result))));
    }

    std::size_t nwritten = static_cast<std::size_t>(req.result);
    uv_fs_req_cleanup(&req);

    return nwritten;
  }
};

struct FlushAwaiter {
  uv_file fd;
  uv_loop_t *loop;
  uv_fs_t req;

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> h) {
    req.data = h.address();

    uv_fs_fsync(loop, &req, fd, [](uv_fs_t *req) {
      auto handle = std::coroutine_handle<>::from_address(req->data);
      handle.resume();
    });
  }

  Expected<void> await_resume() {
    if (req.result < 0) {
      return std::unexpected(IOException("uv_fs_fsync failed: " +
                                         std::string(uv_strerror(req.result))));
    }

    uv_fs_req_cleanup(&req);
    return {};
  }
};

struct StatAwaiter {
  uv_loop_t *loop;
  std::string path;
  uv_fs_t req;
  uv_stat_t statbuf;

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> h) {
    req.data = h.address();
    uv_fs_stat(loop, &req, path.c_str(), [](uv_fs_t *req) {
      auto handle = std::coroutine_handle<>::from_address(req->data);
      handle.resume();
    });
  }

  Expected<uv_stat_t> await_resume() {
    if (req.result < 0) {
      uv_fs_req_cleanup(&req);
      return std::unexpected(IOException("uv_fs_stat failed: " +
                                         std::string(uv_strerror(req.result))));
    }
    uv_stat_t statbuf = req.statbuf;
    uv_fs_req_cleanup(&req);
    return statbuf;
  }
};
} // namespace kl::io
#endif