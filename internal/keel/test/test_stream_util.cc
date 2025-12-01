#include "kl/io/loop.hh"
#include "kl/io/memory_stream.hh"
#include "kl/io/stream_util.hh"
#include <gtest/gtest.h>
#include <span>
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
