#ifndef KL_IO_STREAM_UTIL_HH
#define KL_IO_STREAM_UTIL_HH

#include "kl/concurrent/task.hh"
#include "kl/io/async_stream.hh"

namespace kl::io {
template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<void>>
readExact(std::shared_ptr<Loop> loop, T &stream, std::span<std::byte> buffer) {
  std::size_t totalRead = 0;

  while (totalRead < buffer.size()) {
    auto nread = co_await stream.read(loop, buffer.subspan(totalRead));

    if (!nread) {
      co_return std::unexpected(nread.error());
    }

    if (*nread == 0) {
      co_return std::unexpected(IOException("Unexpected end of stream"));
    }

    totalRead += *nread;
  }

  co_return {};
}
} // namespace kl::io

#endif