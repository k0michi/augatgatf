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
/**
 * @brief Concept for asynchronous readable streams.
 * 
 * The type T must implement the following method:
 * - `kl::concurrent::Task<Expected<std::size_t>> read(std::shared_ptr<kl::io::Loop> loop, std::span<std::byte> buffer)`: Reads data into the provided buffer from the stream asynchronously. Returns the number of bytes read or an IOException on error.
 * - `kl::concurrent::Task<Expected<std::size_t>> read(std::shared_ptr<kl::io::Loop> loop, std::span<std::byte> buffer)`: Reads data into the provided buffer from the stream asynchronously. Returns the number of bytes read or an IOException on error.
 * 
 * @see https://doc.rust-lang.org/std/io/trait.Read.html
 * @see https://docs.rs/tokio/latest/tokio/io/trait.AsyncRead.html
 */
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

/**
 * @brief Concept for asynchronous writable streams.
 * 
 * The type T must implement the following methods:
 * - `kl::concurrent::Task<Expected<std::size_t>> write(std::shared_ptr<kl::io::Loop> loop, std::span<const std::byte> buffer)`: Writes data from the provided buffer to the stream asynchronously. Returns the number of bytes written or an IOException on error.
 * - `kl::concurrent::Task<Expected<void>> flush(std::shared_ptr<kl::io::Loop> loop)`: Flushes any buffered data to the underlying stream asynchronously. Returns void on success or an IOException on error.
 * 
 * @see https://doc.rust-lang.org/std/io/trait.Write.html
 * @see https://docs.rs/tokio/latest/tokio/io/trait.AsyncWrite.html
 */
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

/**
 * @brief Concept for asynchronous seekable streams.
 * 
 * The type T must implement the following method:
 * - `kl::concurrent::Task<std::uint64_t> seek(std::shared_ptr<kl::io::Loop> loop, std::int64_t offset, SeekDirection dir)`: Seeks to a new position in the stream based on the given offset and direction. Returns the new position in the stream.
 * 
 * @see https://doc.rust-lang.org/std/io/trait.Seek.html
 * @see https://docs.rs/tokio/latest/tokio/io/trait.AsyncSeek.html
 */
template <typename T>
concept AsyncSeekable = requires(T &stream, std::shared_ptr<kl::io::Loop> loop,
                                 std::int64_t offset, SeekDirection dir) {
  {
    stream.seek(loop, offset, dir)
  } -> std::same_as<kl::concurrent::Task<std::uint64_t>>;
};

class IAsyncSeekable {
public:
  virtual ~IAsyncSeekable() = default;

  virtual kl::concurrent::Task<std::uint64_t>
  seek(std::shared_ptr<kl::io::Loop> loop, std::int64_t offset,
       SeekDirection dir) = 0;
};
} // namespace kl::io
#endif // KL_IO_ASYNC_STREAM_HH