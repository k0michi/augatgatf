#include "kl/io/loop.hh"
#include "kl/io/memory_stream.hh"
#include "kl/io/stream_util.hh"
#include <gtest/gtest.h>
#include <span>
#include <string>
#include <vector>

using namespace kl::io;

class StreamUtilTest : public ::testing::Test {
protected:
  std::shared_ptr<Loop> loop = Loop::getDefault();
};

TEST_F(StreamUtilTest, ReadExactSuccess) {
  MemoryStream stream;
  std::vector<std::byte> data = {std::byte{0x10}, std::byte{0x20},
                                 std::byte{0x30}};
  std::vector<std::byte> buffer(3);
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await stream.write(loop, std::span<const std::byte>(data));
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result =
        co_await readExact(loop, stream, std::span<std::byte>(buffer));
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(*result, buffer.size());
    EXPECT_EQ(buffer, data);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, ReadExactUnexpectedEnd) {
  MemoryStream stream;
  std::vector<std::byte> data = {std::byte{0x10}, std::byte{0x20}};
  std::vector<std::byte> buffer(3);
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await stream.write(loop, std::span<const std::byte>(data));
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result =
        co_await readExact(loop, stream, std::span<std::byte>(buffer));
    EXPECT_FALSE(result.has_value());
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Uint8) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeUint8(loop, stream, 0xAB);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result = co_await readUint8(loop, stream);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(*result, 0xAB);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Uint16LE_BE) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeUint16(loop, stream, 0x1234, std::endian::little);
    co_await writeUint16(loop, stream, 0xABCD, std::endian::big);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result1 = co_await readUint16(loop, stream, std::endian::little);
    auto result2 = co_await readUint16(loop, stream, std::endian::big);
    EXPECT_TRUE(result1.has_value());
    EXPECT_TRUE(result2.has_value());
    EXPECT_EQ(*result1, 0x1234);
    EXPECT_EQ(*result2, 0xABCD);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Uint32LE_BE) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeUint32(loop, stream, 0x12345678, std::endian::little);
    co_await writeUint32(loop, stream, 0xABCDEF01, std::endian::big);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result1 = co_await readUint32(loop, stream, std::endian::little);
    auto result2 = co_await readUint32(loop, stream, std::endian::big);
    EXPECT_TRUE(result1.has_value());
    EXPECT_TRUE(result2.has_value());
    EXPECT_EQ(*result1, 0x12345678);
    EXPECT_EQ(*result2, 0xABCDEF01);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Uint64LE_BE) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeUint64(loop, stream, 0x1122334455667788, std::endian::little);
    co_await writeUint64(loop, stream, 0xAABBCCDDEEFF0011, std::endian::big);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result1 = co_await readUint64(loop, stream, std::endian::little);
    auto result2 = co_await readUint64(loop, stream, std::endian::big);
    EXPECT_TRUE(result1.has_value());
    EXPECT_TRUE(result2.has_value());
    EXPECT_EQ(*result1, 0x1122334455667788);
    EXPECT_EQ(*result2, 0xAABBCCDDEEFF0011);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Int8) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeUint8(loop, stream, static_cast<std::uint8_t>(-42));
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result = co_await readInt8(loop, stream);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(*result, -42);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Int16LE_BE) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeUint16(loop, stream, static_cast<std::uint16_t>(-12345),
                         std::endian::little);
    co_await writeUint16(loop, stream, static_cast<std::uint16_t>(-23456),
                         std::endian::big);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result1 = co_await readInt16(loop, stream, std::endian::little);
    auto result2 = co_await readInt16(loop, stream, std::endian::big);
    EXPECT_TRUE(result1.has_value());
    EXPECT_TRUE(result2.has_value());
    EXPECT_EQ(*result1, -12345);
    EXPECT_EQ(*result2, -23456);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Int32LE_BE) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeUint32(loop, stream, static_cast<std::uint32_t>(-123456789),
                         std::endian::little);
    co_await writeUint32(loop, stream, static_cast<std::uint32_t>(-987654321),
                         std::endian::big);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result1 = co_await readInt32(loop, stream, std::endian::little);
    auto result2 = co_await readInt32(loop, stream, std::endian::big);
    EXPECT_TRUE(result1.has_value());
    EXPECT_TRUE(result2.has_value());
    EXPECT_EQ(*result1, -123456789);
    EXPECT_EQ(*result2, -987654321);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Int64LE_BE) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeUint64(loop, stream,
                         static_cast<std::uint64_t>(-1234567890123456789),
                         std::endian::little);
    co_await writeUint64(loop, stream,
                         static_cast<std::uint64_t>(-987654321098765432),
                         std::endian::big);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result1 = co_await readInt64(loop, stream, std::endian::little);
    auto result2 = co_await readInt64(loop, stream, std::endian::big);
    EXPECT_TRUE(result1.has_value());
    EXPECT_TRUE(result2.has_value());
    EXPECT_EQ(*result1, -1234567890123456789);
    EXPECT_EQ(*result2, -987654321098765432);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Float32LE_BE) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeFloat32(loop, stream, 3.14159f, std::endian::little);
    co_await writeFloat32(loop, stream, -2.71828f, std::endian::big);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result1 = co_await readFloat32(loop, stream, std::endian::little);
    auto result2 = co_await readFloat32(loop, stream, std::endian::big);
    EXPECT_TRUE(result1.has_value());
    EXPECT_TRUE(result2.has_value());
    EXPECT_FLOAT_EQ(*result1, 3.14159f);
    EXPECT_FLOAT_EQ(*result2, -2.71828f);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, Float64LE_BE) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await writeFloat64(loop, stream, 1.23456789012345, std::endian::little);
    co_await writeFloat64(loop, stream, -9.87654321098765, std::endian::big);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result1 = co_await readFloat64(loop, stream, std::endian::little);
    auto result2 = co_await readFloat64(loop, stream, std::endian::big);
    EXPECT_TRUE(result1.has_value());
    EXPECT_TRUE(result2.has_value());
    EXPECT_DOUBLE_EQ(*result1, 1.23456789012345);
    EXPECT_DOUBLE_EQ(*result2, -9.87654321098765);
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, WriteReadString) {
  MemoryStream stream;
  std::u8string testStr = u8"あいうabcXYZ012!@#";
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await kl::io::writeString(loop, stream, testStr);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result = co_await kl::io::readString(loop, stream, testStr.size());
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(*result, testStr);
    co_return;
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, WriteReadString_Empty) {
  MemoryStream stream;
  std::u8string testStr = u8"";
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await kl::io::writeString(loop, stream, testStr);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result = co_await kl::io::readString(loop, stream, testStr.size());
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(*result, testStr);
    co_return;
  };
  task();
  loop->run();
}

TEST_F(StreamUtilTest, ReadString_Partial) {
  MemoryStream stream;
  std::u8string testStr = u8"abcdefg";
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await kl::io::writeString(loop, stream, testStr);
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto result = co_await kl::io::readString(loop, stream, 3);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(*result, u8"abc");
    co_return;
  };
  task();
  loop->run();
}