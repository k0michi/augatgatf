#include "kl/io/file_system.hh"

#include <string>

#include <uv.h>

struct ReadFileAwaiter {
  ReadFileAwaiter(std::string_view filename) : mFilename(filename) {}

  bool await_ready() const noexcept { return false; }

  static void onRead(uv_fs_t *req) {
    auto self = static_cast<ReadFileAwaiter *>(req->data);

    if (req->result < 0) {
      self->mError = true;
      self->mHandle.resume();
      uv_fs_req_cleanup(req);
      return;
    } else if (req->result == 0) {
      // EOF
      uv_fs_t *closeReq = &self->mCloseReq;
      closeReq->data = self;
      uv_fs_close(uv_default_loop(), closeReq, self->mFile, [](uv_fs_t *req) {
        auto self = static_cast<ReadFileAwaiter *>(req->data);
        uv_fs_req_cleanup(req);
        self->mHandle.resume();
      });
      uv_fs_req_cleanup(req);
      return;
    } else {
      size_t nread = static_cast<size_t>(req->result);
      self->mBuffer.insert(
          self->mBuffer.end(), reinterpret_cast<std::byte *>(self->mBuf.base),
          reinterpret_cast<std::byte *>(self->mBuf.base) + nread);
    }

    uv_fs_req_cleanup(req);
    req->data = self;
    // Continue reading
    uv_fs_read(uv_default_loop(), req, self->mFile, &self->mBuf, 1, -1, onRead);
  }

  void await_suspend(std::coroutine_handle<> handle) {
    mHandle = handle;

    uv_loop_t *loop = uv_default_loop();
    mReq.data = this;
    uv_fs_open(loop, &mReq, mFilename.c_str(), O_RDONLY, 0, [](uv_fs_t *req) {
      auto self = static_cast<ReadFileAwaiter *>(req->data);
      if (req->result < 0) {
        self->mError = true;
        self->mHandle.resume();
        uv_fs_req_cleanup(req);
        return;
      }

      self->mFile = static_cast<uv_file>(req->result);
      uv_fs_req_cleanup(req);
      self->mBuf =
          uv_buf_init(static_cast<char *>(malloc(64 * 1024)), 64 * 1024);
      self->mBuffer.reserve(64 * 1024);

      req->data = self;
      uv_fs_read(uv_default_loop(), req, self->mFile, &self->mBuf, 1, -1,
                 onRead);
    });
  }

  std::vector<std::byte> await_resume() {
    // TODO:
    // if (mError) {
    //   throw std::runtime_error("Failed to read file: " + mFilename);
    // }
    return mBuffer;
  }

private:
  std::string mFilename;
  std::vector<std::byte> mBuffer;
  uv_fs_t mReq;
  uv_fs_t mCloseReq;
  uv_file mFile = -1;
  uv_buf_t mBuf;
  std::coroutine_handle<> mHandle;
  bool mError = false;
};

namespace kl::io {
kl::platform::Task<std::vector<std::byte>>
FileSystem::readFileBinaryAsync(std::string_view filename) {
  std::vector<std::byte> result = co_await ReadFileAwaiter(filename);
  co_return result;
}
} // namespace kl::io