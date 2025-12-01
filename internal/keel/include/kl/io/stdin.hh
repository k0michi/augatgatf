#ifndef KL_IO_STDIN_HH
#define KL_IO_STDIN_HH

#include <memory>
#include <span>
#include <string_view>

#include <uv.h>

#include "kl/concurrent/task.hh"
#include "kl/io/async_stream.hh"
#include "kl/io/loop.hh"

namespace kl::io {
class Stdin {
public:
  ~Stdin();

  kl::concurrent::Task<Expected<std::size_t>> read(std::shared_ptr<Loop> loop,
                                                   std::span<std::byte> buffer);
};

inline std::shared_ptr<Stdin> in = std::make_shared<Stdin>();
} // namespace kl::io
#endif