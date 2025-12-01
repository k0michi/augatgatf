#ifndef KL_IO_STDERR_HH
#define KL_IO_STDERR_HH

#include <memory>
#include <span>
#include <string_view>

#include <uv.h>

#include "kl/concurrent/task.hh"
#include "kl/io/async_stream.hh"
#include "kl/io/loop.hh"

namespace kl::io {
class Stderr {
public:
  ~Stderr();

  kl::concurrent::Task<Expected<std::size_t>>
  write(std::shared_ptr<Loop> loop, std::span<const std::byte> buffer);
  kl::concurrent::Task<Expected<void>> flush(std::shared_ptr<Loop> loop);
};

inline std::shared_ptr<Stderr> out = std::make_shared<Stderr>();
} // namespace kl::io
#endif