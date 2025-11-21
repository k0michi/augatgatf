#ifndef KL_IO_FILE_HH
#define KL_IO_FILE_HH

#include <memory>
#include <span>
#include <string_view>

#include <uv.h>

#include "kl/concurrent/task.hh"
#include "kl/io/async_stream.hh"
#include "kl/io/loop.hh"
#include "kl/io/open_flag.hh"

namespace kl::io {
class File {
public:
  ~File();

  kl::concurrent::Task<Expected<std::size_t>> read(std::shared_ptr<Loop> loop,
                                                   std::span<std::byte> buffer);
  kl::concurrent::Task<Expected<std::size_t>>
  write(std::shared_ptr<Loop> loop, std::span<const std::byte> buffer);
  kl::concurrent::Task<Expected<void>> flush(std::shared_ptr<Loop> loop);
  kl::concurrent::Task<Expected<std::int64_t>>
  seek(std::shared_ptr<Loop> loop, std::int64_t offset, SeekDirection dir);

  static kl::concurrent::Task<Expected<std::shared_ptr<File>>>
  open(std::string_view path, OpenFlag flags = OpenFlag::eRdOnly,
       std::uint32_t mode = 0666);
  static kl::concurrent::Task<Expected<std::shared_ptr<File>>>
  open(std::shared_ptr<Loop> loop, std::string_view path,
       OpenFlag flags = OpenFlag::eRdOnly, std::uint32_t mode = 0666);
  kl::concurrent::Task<Expected<void>> close(std::shared_ptr<Loop> loop);
  kl::concurrent::Task<Expected<void>> close();

private:
  uv_file fd_;
  std::string path_;
  OpenFlag flags_;
  std::uint32_t mode_;
  std::uint64_t offset_ = 0;
};
} // namespace kl::io
#endif // KL_IO_FILE_HH