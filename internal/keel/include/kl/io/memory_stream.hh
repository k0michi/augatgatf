#ifndef KL_IO_MEMORY_STREAM_HH
#define KL_IO_MEMORY_STREAM_HH

#include <memory>
#include <span>
#include <vector>

#include "kl/concurrent/task.hh"
#include "kl/io/async_stream.hh"
#include "kl/io/loop.hh"

namespace kl::io {
class MemoryStream {
public:
  MemoryStream();
  ~MemoryStream();

  kl::concurrent::Task<Expected<std::size_t>> read(std::shared_ptr<Loop> loop,
                                                   std::span<std::byte> buffer);
  kl::concurrent::Task<Expected<std::size_t>>
  write(std::shared_ptr<Loop> loop, std::span<const std::byte> buffer);
  kl::concurrent::Task<Expected<void>> flush(std::shared_ptr<Loop> loop);
  kl::concurrent::Task<Expected<std::int64_t>>
  seek(std::shared_ptr<Loop> loop, std::int64_t offset, SeekDirection dir);

private:
  std::vector<std::byte> buffer_;
  std::uint64_t offset_ = 0;
};
} // namespace kl::io

#endif