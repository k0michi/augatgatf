#ifndef KL_IO_STDOUT_HH
#define KL_IO_STDOUT_HH

#include <memory>
#include <span>
#include <string_view>

#include <uv.h>

#include "kl/concurrent/task.hh"
#include "kl/io/async_stream.hh"
#include "kl/io/loop.hh"

namespace kl::io {
class Stdout {
public:
  ~Stdout();

  kl::concurrent::Task<Expected<std::size_t>>
  write(std::shared_ptr<Loop> loop, std::span<const std::byte> buffer);
  kl::concurrent::Task<Expected<void>> flush(std::shared_ptr<Loop> loop);
};

inline std::shared_ptr<Stdout> out = std::make_shared<Stdout>();
} // namespace kl::io
#endif