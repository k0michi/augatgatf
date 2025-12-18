#include "kl/io/loop.hh"
#include "kl/io/memory_stream.hh"
#include <gtest/gtest.h>
#include <span>
#include <vector>

using namespace kl::io;

class MemoryStreamTest : public ::testing::Test {
protected:
  std::shared_ptr<Loop> loop = Loop::create();
};

TEST_F(MemoryStreamTest, WriteAndRead) {
  MemoryStream stream;
  std::vector<std::byte> data = {std::byte{0x11}, std::byte{0x22},
                                 std::byte{0x33}, std::byte{0x44}};
  std::vector<std::byte> buffer(4);
  auto task = [&]() -> kl::concurrent::Task<void> {
    auto writeResult =
        co_await stream.write(loop, std::span<const std::byte>(data));
    EXPECT_TRUE(writeResult.has_value());
    EXPECT_EQ(*writeResult, data.size());
    auto seekResult = co_await stream.seek(loop, 0, SeekDirection::eBegin);
    EXPECT_TRUE(seekResult.has_value());
    auto readResult = co_await stream.read(loop, std::span<std::byte>(buffer));
    EXPECT_TRUE(readResult.has_value());
    EXPECT_EQ(*readResult, data.size());
    EXPECT_EQ(buffer, data);
  };
  task();
  loop->run();
}

TEST_F(MemoryStreamTest, SeekAndWrite) {
  MemoryStream stream;
  std::vector<std::byte> data1 = {std::byte{0xAA}, std::byte{0xBB}};
  std::vector<std::byte> data2 = {std::byte{0xCC}, std::byte{0xDD}};
  std::vector<std::byte> buffer(4);
  auto task = [&]() -> kl::concurrent::Task<void> {
    co_await stream.write(loop, std::span<const std::byte>(data1));
    co_await stream.seek(loop, 2, SeekDirection::eBegin);
    co_await stream.write(loop, std::span<const std::byte>(data2));
    co_await stream.seek(loop, 0, SeekDirection::eBegin);
    auto readResult = co_await stream.read(loop, std::span<std::byte>(buffer));
    EXPECT_TRUE(readResult.has_value());
    EXPECT_EQ(buffer[0], std::byte{0xAA});
    EXPECT_EQ(buffer[1], std::byte{0xBB});
    EXPECT_EQ(buffer[2], std::byte{0xCC});
    EXPECT_EQ(buffer[3], std::byte{0xDD});
  };
  task();
  loop->run();
}

TEST_F(MemoryStreamTest, FlushDoesNothing) {
  MemoryStream stream;
  auto task = [&]() -> kl::concurrent::Task<void> {
    auto flushResult = co_await stream.flush(loop);
    EXPECT_TRUE(flushResult.has_value());
  };
  task();
  loop->run();
}
