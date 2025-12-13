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

class IAsyncReadable {
public:
  virtual ~IAsyncReadable() = default;

  virtual kl::concurrent::Task<Expected<std::size_t>>
  read(std::shared_ptr<kl::io::Loop> loop, std::span<std::byte> buffer) = 0;
};

template <typename T>
concept AsyncWritable = requires(T &stream, std::shared_ptr<kl::io::Loop> loop,
                                 std::span<const std::byte> buffer) {
  {
    stream.write(loop, buffer)
  } -> std::same_as<kl::concurrent::Task<Expected<std::size_t>>>;
  { stream.flush(loop) } -> std::same_as<kl::concurrent::Task<Expected<void>>>;
};

class IAsyncWritable {
public:
  virtual ~IAsyncWritable() = default;

  virtual kl::concurrent::Task<Expected<std::size_t>>
  write(std::shared_ptr<kl::io::Loop> loop,
        std::span<const std::byte> buffer) = 0;
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

class IAsyncSeekable {
public:
  virtual ~IAsyncSeekable() = default;

  virtual kl::concurrent::Task<std::int64_t>
  seek(std::shared_ptr<kl::io::Loop> loop, std::int64_t offset,
       SeekDirection dir) = 0;
};
} // namespace kl::io
#endif // KL_IO_ASYNC_STREAM_HH