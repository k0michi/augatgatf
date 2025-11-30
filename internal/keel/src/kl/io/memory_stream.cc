#include "kl/io/memory_stream.hh"

namespace kl::io {
MemoryStream::MemoryStream() = default;

MemoryStream::~MemoryStream() = default;

kl::concurrent::Task<Expected<std::size_t>>
MemoryStream::read(std::shared_ptr<Loop> loop, std::span<std::byte> buffer) {
  std::size_t nread = 0;
  std::size_t available = buffer_.size() - offset_;
  nread = std::min(buffer.size(), available);
  std::memcpy(buffer.data(), buffer_.data() + offset_, nread);
  offset_ += nread;
  co_return nread;
}

kl::concurrent::Task<Expected<std::size_t>>
MemoryStream::write(std::shared_ptr<Loop> loop,
                    std::span<const std::byte> buffer) {
  if (offset_ + buffer.size() > buffer_.size()) {
    buffer_.resize(offset_ + buffer.size());
  }
  std::memcpy(buffer_.data() + offset_, buffer.data(), buffer.size());
  offset_ += buffer.size();
  co_return buffer.size();
}

kl::concurrent::Task<Expected<void>>
MemoryStream::flush(std::shared_ptr<Loop> loop) {
  co_return {};
}

kl::concurrent::Task<Expected<std::int64_t>>
MemoryStream::seek(std::shared_ptr<Loop> loop, std::int64_t offset,
                   SeekDirection dir) {
  switch (dir) {
  case SeekDirection::eBegin:
    offset_ = offset;
    break;
  case SeekDirection::eCurrent:
    offset_ += offset;
    break;
  case SeekDirection::eEnd:
    offset_ = buffer_.size() + offset;
    break;
  }

  co_return offset_;
}
} // namespace kl::io