#ifndef KL_IO_STREAM_UTIL_HH
#define KL_IO_STREAM_UTIL_HH

#include <array>
#include <bit>

#include "kl/concurrent/task.hh"
#include "kl/io/async_stream.hh"

namespace kl::io {
template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::size_t>>
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

  co_return totalRead;
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::uint8_t>>
readUint8(std::shared_ptr<Loop> loop, T &stream) {
  std::array<std::byte, 1> buffer;
  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  co_return static_cast<std::uint8_t>(buffer[0]);
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::uint16_t>>
readUint16(std::shared_ptr<Loop> loop, T &stream, std::endian endian) {
  std::array<std::byte, 2> buffer;
  auto result = co_await readExact(loop, stream, std::span{buffer});
  if (!result) {
    co_return std::unexpected(result.error());
  }

  if (endian == std::endian::little) {
    co_return static_cast<std::uint16_t>(buffer[0]) |
        (static_cast<std::uint16_t>(buffer[1]) << 8);
  } else {
    co_return (static_cast<std::uint16_t>(buffer[0]) << 8) |
        static_cast<std::uint16_t>(buffer[1]);
  }
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::uint32_t>>
readUint32(std::shared_ptr<Loop> loop, T &stream, std::endian endian) {
  std::array<std::byte, 4> buffer;
  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  if (endian == std::endian::little) {
    co_return static_cast<std::uint32_t>(buffer[0]) |
        (static_cast<std::uint32_t>(buffer[1]) << 8) |
        (static_cast<std::uint32_t>(buffer[2]) << 16) |
        (static_cast<std::uint32_t>(buffer[3]) << 24);
  } else {
    co_return (static_cast<std::uint32_t>(buffer[0]) << 24) |
        (static_cast<std::uint32_t>(buffer[1]) << 16) |
        (static_cast<std::uint32_t>(buffer[2]) << 8) |
        static_cast<std::uint32_t>(buffer[3]);
  }
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::uint64_t>>
readUint64(std::shared_ptr<Loop> loop, T &stream, std::endian endian) {
  std::array<std::byte, 8> buffer;

  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  if (endian == std::endian::little) {
    co_return static_cast<std::uint64_t>(buffer[0]) |
        (static_cast<std::uint64_t>(buffer[1]) << 8) |
        (static_cast<std::uint64_t>(buffer[2]) << 16) |
        (static_cast<std::uint64_t>(buffer[3]) << 24) |
        (static_cast<std::uint64_t>(buffer[4]) << 32) |
        (static_cast<std::uint64_t>(buffer[5]) << 40) |
        (static_cast<std::uint64_t>(buffer[6]) << 48) |
        (static_cast<std::uint64_t>(buffer[7]) << 56);
  } else {
    co_return (static_cast<std::uint64_t>(buffer[0]) << 56) |
        (static_cast<std::uint64_t>(buffer[1]) << 48) |
        (static_cast<std::uint64_t>(buffer[2]) << 40) |
        (static_cast<std::uint64_t>(buffer[3]) << 32) |
        (static_cast<std::uint64_t>(buffer[4]) << 24) |
        (static_cast<std::uint64_t>(buffer[5]) << 16) |
        (static_cast<std::uint64_t>(buffer[6]) << 8) |
        static_cast<std::uint64_t>(buffer[7]);
  }
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::int8_t>> readInt8(std::shared_ptr<Loop> loop,
                                                     T &stream) {
  std::array<std::byte, 1> buffer;
  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  co_return static_cast<std::int8_t>(buffer[0]);
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::int16_t>>
readInt16(std::shared_ptr<Loop> loop, T &stream, std::endian endian) {
  std::array<std::byte, 2> buffer;
  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  if (endian == std::endian::little) {
    co_return static_cast<std::int16_t>(
        static_cast<std::uint16_t>(buffer[0]) |
        (static_cast<std::uint16_t>(buffer[1]) << 8));
  } else {
    co_return static_cast<std::int16_t>(
        (static_cast<std::uint16_t>(buffer[0]) << 8) |
        static_cast<std::uint16_t>(buffer[1]));
  }
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::int32_t>>
readInt32(std::shared_ptr<Loop> loop, T &stream, std::endian endian) {
  std::array<std::byte, 4> buffer;
  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  if (endian == std::endian::little) {
    co_return static_cast<std::int32_t>(
        static_cast<std::uint32_t>(buffer[0]) |
        (static_cast<std::uint32_t>(buffer[1]) << 8) |
        (static_cast<std::uint32_t>(buffer[2]) << 16) |
        (static_cast<std::uint32_t>(buffer[3]) << 24));
  } else {
    co_return static_cast<std::int32_t>(
        (static_cast<std::uint32_t>(buffer[0]) << 24) |
        (static_cast<std::uint32_t>(buffer[1]) << 16) |
        (static_cast<std::uint32_t>(buffer[2]) << 8) |
        static_cast<std::uint32_t>(buffer[3]));
  }
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<std::int64_t>>
readInt64(std::shared_ptr<Loop> loop, T &stream, std::endian endian) {
  std::array<std::byte, 8> buffer;

  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  if (endian == std::endian::little) {
    co_return static_cast<std::int64_t>(
        static_cast<std::uint64_t>(buffer[0]) |
        (static_cast<std::uint64_t>(buffer[1]) << 8) |
        (static_cast<std::uint64_t>(buffer[2]) << 16) |
        (static_cast<std::uint64_t>(buffer[3]) << 24) |
        (static_cast<std::uint64_t>(buffer[4]) << 32) |
        (static_cast<std::uint64_t>(buffer[5]) << 40) |
        (static_cast<std::uint64_t>(buffer[6]) << 48) |
        (static_cast<std::uint64_t>(buffer[7]) << 56));
  } else {
    co_return static_cast<std::int64_t>(
        (static_cast<std::uint64_t>(buffer[0]) << 56) |
        (static_cast<std::uint64_t>(buffer[1]) << 48) |
        (static_cast<std::uint64_t>(buffer[2]) << 40) |
        (static_cast<std::uint64_t>(buffer[3]) << 32) |
        (static_cast<std::uint64_t>(buffer[4]) << 24) |
        (static_cast<std::uint64_t>(buffer[5]) << 16) |
        (static_cast<std::uint64_t>(buffer[6]) << 8) |
        static_cast<std::uint64_t>(buffer[7]));
  }
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<float>>
readFloat32(std::shared_ptr<Loop> loop, T &stream, std::endian endian) {
  std::array<std::byte, 4> buffer;
  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  std::uint32_t intValue;

  if (endian == std::endian::little) {
    intValue = static_cast<std::uint32_t>(buffer[0]) |
               (static_cast<std::uint32_t>(buffer[1]) << 8) |
               (static_cast<std::uint32_t>(buffer[2]) << 16) |
               (static_cast<std::uint32_t>(buffer[3]) << 24);
  } else {
    intValue = (static_cast<std::uint32_t>(buffer[0]) << 24) |
               (static_cast<std::uint32_t>(buffer[1]) << 16) |
               (static_cast<std::uint32_t>(buffer[2]) << 8) |
               static_cast<std::uint32_t>(buffer[3]);
  }

  float value = std::bit_cast<float>(intValue);
  co_return value;
}

template <kl::io::AsyncReadable T>
kl::concurrent::Task<Expected<double>>
readFloat64(std::shared_ptr<Loop> loop, T &stream, std::endian endian) {
  std::array<std::byte, 8> buffer;
  auto result = co_await readExact(loop, stream, std::span{buffer});

  if (!result) {
    co_return std::unexpected(result.error());
  }

  std::uint64_t intValue;

  if (endian == std::endian::little) {
    intValue = static_cast<std::uint64_t>(buffer[0]) |
               (static_cast<std::uint64_t>(buffer[1]) << 8) |
               (static_cast<std::uint64_t>(buffer[2]) << 16) |
               (static_cast<std::uint64_t>(buffer[3]) << 24) |
               (static_cast<std::uint64_t>(buffer[4]) << 32) |
               (static_cast<std::uint64_t>(buffer[5]) << 40) |
               (static_cast<std::uint64_t>(buffer[6]) << 48) |
               (static_cast<std::uint64_t>(buffer[7]) << 56);
  } else {
    intValue = (static_cast<std::uint64_t>(buffer[0]) << 56) |
               (static_cast<std::uint64_t>(buffer[1]) << 48) |
               (static_cast<std::uint64_t>(buffer[2]) << 40) |
               (static_cast<std::uint64_t>(buffer[3]) << 32) |
               (static_cast<std::uint64_t>(buffer[4]) << 24) |
               (static_cast<std::uint64_t>(buffer[5]) << 16) |
               (static_cast<std::uint64_t>(buffer[6]) << 8) |
               static_cast<std::uint64_t>(buffer[7]);
  }

  double value = std::bit_cast<double>(intValue);
  co_return value;
}

template <kl::io::AsyncWritable T>
kl::concurrent::Task<Expected<void>> writeUint8(std::shared_ptr<Loop> loop,
                                                T &stream, std::uint8_t value) {
  std::array<std::byte, 1> buffer;
  buffer[0] = static_cast<std::byte>(value);
  co_await stream.write(loop, std::span{buffer});
  co_return {};
}

template <kl::io::AsyncWritable T>
kl::concurrent::Task<Expected<void>> writeUint16(std::shared_ptr<Loop> loop,
                                                 T &stream, std::uint16_t value,
                                                 std::endian endian) {
  std::array<std::byte, 2> buffer;

  if (endian == std::endian::little) {
    buffer[0] = static_cast<std::byte>(value & 0xFF);
    buffer[1] = static_cast<std::byte>((value >> 8) & 0xFF);
  } else {
    buffer[0] = static_cast<std::byte>((value >> 8) & 0xFF);
    buffer[1] = static_cast<std::byte>(value & 0xFF);
  }

  co_await stream.write(loop, std::span{buffer});
  co_return {};
}

template <kl::io::AsyncWritable T>
kl::concurrent::Task<Expected<void>> writeUint32(std::shared_ptr<Loop> loop,
                                                 T &stream, std::uint32_t value,
                                                 std::endian endian) {
  std::array<std::byte, 4> buffer;

  if (endian == std::endian::little) {
    buffer[0] = static_cast<std::byte>(value & 0xFF);
    buffer[1] = static_cast<std::byte>((value >> 8) & 0xFF);
    buffer[2] = static_cast<std::byte>((value >> 16) & 0xFF);
    buffer[3] = static_cast<std::byte>((value >> 24) & 0xFF);
  } else {
    buffer[0] = static_cast<std::byte>((value >> 24) & 0xFF);
    buffer[1] = static_cast<std::byte>((value >> 16) & 0xFF);
    buffer[2] = static_cast<std::byte>((value >> 8) & 0xFF);
    buffer[3] = static_cast<std::byte>(value & 0xFF);
  }

  co_await stream.write(loop, std::span{buffer});
  co_return {};
}

template <kl::io::AsyncWritable T>
kl::concurrent::Task<Expected<void>> writeUint64(std::shared_ptr<Loop> loop,
                                                 T &stream, std::uint64_t value,
                                                 std::endian endian) {
  std::array<std::byte, 8> buffer;

  if (endian == std::endian::little) {
    buffer[0] = static_cast<std::byte>(value & 0xFF);
    buffer[1] = static_cast<std::byte>((value >> 8) & 0xFF);
    buffer[2] = static_cast<std::byte>((value >> 16) & 0xFF);
    buffer[3] = static_cast<std::byte>((value >> 24) & 0xFF);
    buffer[4] = static_cast<std::byte>((value >> 32) & 0xFF);
    buffer[5] = static_cast<std::byte>((value >> 40) & 0xFF);
    buffer[6] = static_cast<std::byte>((value >> 48) & 0xFF);
    buffer[7] = static_cast<std::byte>((value >> 56) & 0xFF);
  } else {
    buffer[0] = static_cast<std::byte>((value >> 56) & 0xFF);
    buffer[1] = static_cast<std::byte>((value >> 48) & 0xFF);
    buffer[2] = static_cast<std::byte>((value >> 40) & 0xFF);
    buffer[3] = static_cast<std::byte>((value >> 32) & 0xFF);
    buffer[4] = static_cast<std::byte>((value >> 24) & 0xFF);
    buffer[5] = static_cast<std::byte>((value >> 16) & 0xFF);
    buffer[6] = static_cast<std::byte>((value >> 8) & 0xFF);
    buffer[7] = static_cast<std::byte>(value & 0xFF);
  }

  co_await stream.write(loop, std::span{buffer});
  co_return {};
}

template <kl::io::AsyncWritable T>
kl::concurrent::Task<Expected<void>> writeFloat32(std::shared_ptr<Loop> loop,
                                                  T &stream, float value,
                                                  std::endian endian) {
  std::array<std::byte, 4> buffer;

  std::uint32_t intValue = std::bit_cast<std::uint32_t>(value);

  if (endian == std::endian::little) {
    buffer[0] = static_cast<std::byte>(intValue & 0xFF);
    buffer[1] = static_cast<std::byte>((intValue >> 8) & 0xFF);
    buffer[2] = static_cast<std::byte>((intValue >> 16) & 0xFF);
    buffer[3] = static_cast<std::byte>((intValue >> 24) & 0xFF);
  } else {
    buffer[0] = static_cast<std::byte>((intValue >> 24) & 0xFF);
    buffer[1] = static_cast<std::byte>((intValue >> 16) & 0xFF);
    buffer[2] = static_cast<std::byte>((intValue >> 8) & 0xFF);
    buffer[3] = static_cast<std::byte>(intValue & 0xFF);
  }

  co_await stream.write(loop, std::span{buffer});
  co_return {};
}

template <kl::io::AsyncWritable T>
kl::concurrent::Task<Expected<void>> writeFloat64(std::shared_ptr<Loop> loop,
                                                  T &stream, double value,
                                                  std::endian endian) {
  std::array<std::byte, 8> buffer;

  std::uint64_t intValue = std::bit_cast<std::uint64_t>(value);

  if (endian == std::endian::little) {
    buffer[0] = static_cast<std::byte>(intValue & 0xFF);
    buffer[1] = static_cast<std::byte>((intValue >> 8) & 0xFF);
    buffer[2] = static_cast<std::byte>((intValue >> 16) & 0xFF);
    buffer[3] = static_cast<std::byte>((intValue >> 24) & 0xFF);
    buffer[4] = static_cast<std::byte>((intValue >> 32) & 0xFF);
    buffer[5] = static_cast<std::byte>((intValue >> 40) & 0xFF);
    buffer[6] = static_cast<std::byte>((intValue >> 48) & 0xFF);
    buffer[7] = static_cast<std::byte>((intValue >> 56) & 0xFF);
  } else {
    buffer[0] = static_cast<std::byte>((intValue >> 56) & 0xFF);
    buffer[1] = static_cast<std::byte>((intValue >> 48) & 0xFF);
    buffer[2] = static_cast<std::byte>((intValue >> 40) & 0xFF);
    buffer[3] = static_cast<std::byte>((intValue >> 32) & 0xFF);
    buffer[4] = static_cast<std::byte>((intValue >> 24) & 0xFF);
    buffer[5] = static_cast<std::byte>((intValue >> 16) & 0xFF);
    buffer[6] = static_cast<std::byte>((intValue >> 8) & 0xFF);
    buffer[7] = static_cast<std::byte>(intValue & 0xFF);
  }

  co_await stream.write(loop, std::span{buffer});
  co_return {};
}
} // namespace kl::io
#endif