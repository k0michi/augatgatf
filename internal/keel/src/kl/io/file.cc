#include "kl/io/file.hh"

#include "kl/io/awaiter.hh"

namespace kl::io {
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

kl::concurrent::Task<Expected<std::uint64_t>>
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