#include "kl/io/stdout.hh"
#include <cstring>
#include <gtest/gtest.h>
#include <span>
#include <vector>

using namespace kl::io;

TEST(StdoutTest, WriteFlush) {
  auto loop = Loop::getDefault();
  std::string_view message = "Hello, Stdout!\n";
  std::vector<std::byte> data(message.size());
  std::memcpy(data.data(), message.data(), message.size());

  auto task = [&]() -> kl::concurrent::Task<void> {
    auto writeResult =
        co_await out->write(loop, std::span<const std::byte>(data));
    EXPECT_TRUE(writeResult.has_value());
    EXPECT_EQ(*writeResult, data.size());
    auto flushResult = co_await out->flush(loop);
    EXPECT_TRUE(flushResult.has_value());
  };
  task();
  loop->run();
}
