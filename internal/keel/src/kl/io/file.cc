#include "kl/io/file.hh"

namespace kl::io {
namespace {
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
} // namespace

kl::concurrent::Task<Expected<std::shared_ptr<File>>>
File::open(std::shared_ptr<Loop> loop, std::string_view path, OpenFlag flags,
           std::uint32_t mode) {
  auto uvloop = loop ? loop->getUVLoop() : nullptr;
  OpenAwaiter awaiter{uvloop, std::string(path), static_cast<int>(flags),
                      static_cast<int>(mode)};
  auto result = co_await awaiter;

  if (!result) {
    co_return std::unexpected(result.error());
  } else {
    auto file = std::shared_ptr<File>(new File());
    file->fd_ = *result;
    file->path_ = path;
    file->flags_ = flags;
    file->mode_ = mode;
    co_return file;
  }
}

kl::concurrent::Task<Expected<std::shared_ptr<File>>>
File::open(std::string_view path, OpenFlag flags, std::uint32_t mode) {
  co_return co_await File::open(Loop::getDefault(), path, flags, mode);
}

kl::concurrent::Task<Expected<void>> File::close(std::shared_ptr<Loop> loop) {
  auto uvloop = loop ? loop->getUVLoop() : nullptr;

  if (fd_ < 0) {
    co_return {};
  }

  CloseAwaiter awaiter{fd_, uvloop};
  auto result = co_await awaiter;
  fd_ = -1;
  co_return result;
}

kl::concurrent::Task<Expected<void>> File::close() {
  co_return co_await close(Loop::getDefault());
}

File::~File() {
  if (fd_ >= 0) {
    uv_fs_t req;
    uv_fs_close(nullptr, &req, fd_, nullptr);
    uv_fs_req_cleanup(&req);
  }
}

kl::concurrent::Task<Expected<std::size_t>>
File::read(std::shared_ptr<Loop> loop, std::span<std::byte> buffer) {
  auto uvloop = loop->getUVLoop();

  ReadAwaiter awaiter{fd_,
                      uvloop,
                      buffer.data(),
                      static_cast<unsigned int>(buffer.size()),
                      0,
                      {},
                      static_cast<std::int64_t>(offset_)};
  auto nread = co_await awaiter;

  if (nread) {
    offset_ += *nread;
  }

  co_return nread;
}

kl::concurrent::Task<Expected<std::size_t>>
File::write(std::shared_ptr<Loop> loop, std::span<const std::byte> buffer) {
  auto uvloop = loop->getUVLoop();

  WriteAwaiter awaiter{fd_,
                       uvloop,
                       buffer.data(),
                       static_cast<unsigned int>(buffer.size()),
                       0,
                       {},
                       static_cast<std::int64_t>(offset_)};
  auto nwritten = co_await awaiter;

  if (nwritten) {
    offset_ += *nwritten;
  }

  co_return nwritten;
}

kl::concurrent::Task<Expected<void>> File::flush(std::shared_ptr<Loop> loop) {
  auto uvloop = loop->getUVLoop();

  FlushAwaiter awaiter{fd_, uvloop, {}};
  co_return co_await awaiter;
}

kl::concurrent::Task<Expected<std::int64_t>>
File::seek(std::shared_ptr<Loop> loop, std::int64_t offset, SeekDirection dir) {
  switch (dir) {
  case SeekDirection::eBegin:
    offset_ = offset;
    break;
  case SeekDirection::eCurrent:
    offset_ += offset;
    break;
  case SeekDirection::eEnd: {
    StatAwaiter awaiter{loop->getUVLoop(), path_, {}};
    auto statResult = co_await awaiter;

    if (!statResult) {
      co_return std::unexpected(statResult.error());
    }

    offset_ = static_cast<std::uint64_t>(statResult->st_size) + offset;
    break;
  }
  }

  co_return offset_;
}
} // namespace kl::io