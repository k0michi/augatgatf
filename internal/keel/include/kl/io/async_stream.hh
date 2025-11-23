#ifndef KL_IO_ASYNC_STREAM_HH
#define KL_IO_ASYNC_STREAM_HH

#include <concepts>
#include <cstdint>
#include <expected>
#include <span>
#include <stdexcept>
#include <string>

#include "kl/concurrent/task.hh"
#include "kl/io/io_exception.hh"
#include "kl/io/loop.hh"

namespace kl::io {
template <typename T>
concept AsyncReadable = requires(T &stream, std::shared_ptr<kl::io::Loop> loop,
                                 std::span<std::byte> buffer) {
  {
    stream.read(loop, buffer)
  } -> std::same_as<kl::concurrent::Task<Expected<std::size_t>>>;
};

template <typename T>
concept AsyncWritable = requires(T &stream, std::shared_ptr<kl::io::Loop> loop,
                                 std::span<const std::byte> buffer) {
  {
    stream.write(loop, buffer)
  } -> std::same_as<kl::concurrent::Task<Expected<std::size_t>>>;
  { stream.flush(loop) } -> std::same_as<kl::concurrent::Task<Expected<void>>>;
};

enum class SeekDirection {
  eBegin,
  eCurrent,
  eEnd,
};

template <typename T>
concept AsyncSeekable = requires(T &stream, std::shared_ptr<kl::io::Loop> loop,
                                 std::int64_t offset, SeekDirection dir) {
  {
    stream.seek(loop, offset, dir)
  } -> std::same_as<kl::concurrent::Task<std::int64_t>>;
};
} // namespace kl::io
#endif // KL_IO_ASYNC_STREAM_HH