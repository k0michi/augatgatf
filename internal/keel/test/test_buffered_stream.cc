#include <iostream>

#include <gtest/gtest.h>

#include "kl/io/buffered_stream.hh"
#include "util.hh"

using namespace kl::io;

class BufferedStreamSeedTest : public ::testing::TestWithParam<uint_fast32_t> {
};

TEST_P(BufferedStreamSeedTest, BufferedReader) {
  auto loop = Loop::create();

  [&]() -> kl::concurrent::Task<void> {
    std::string_view data = "Hello, BufferedReader!";
    uint_fast32_t seed = GetParam();
    RandomizedAsyncReadable randReadable(std::string{data}, seed);
    kl::io::BufferedReader bufferedReader{randReadable};
    std::vector<std::byte> buffer(10);
    std::string result;

    std::size_t nread;
    do {
      auto readResult = co_await bufferedReader.read(loop, std::span{buffer});
      EXPECT_TRUE(readResult.has_value());
      nread = *readResult;
      result.append(reinterpret_cast<const char *>(buffer.data()), nread);
    } while (nread > 0);

    EXPECT_EQ(result, "Hello, BufferedReader!");
  }();

  loop->run();
}

INSTANTIATE_TEST_SUITE_P(Seeds, BufferedStreamSeedTest,
                         ::testing::Range<uint_fast32_t>(0, 100));

TEST_P(BufferedStreamSeedTest, BufferedWriter) {
  auto loop = Loop::create();

  [&]() -> kl::concurrent::Task<void> {
    std::string_view data = "Hello, BufferedWriter!";
    uint_fast32_t seed = GetParam();
    RandomizedAsyncWritable randWritable(seed);
    kl::io::BufferedWriter bufferedWriter{randWritable};
    bufferedWriter.write(
        loop, std::span{reinterpret_cast<const std::byte *>(data.data()),
                        data.size()});
    auto flushResult = co_await bufferedWriter.flush(loop);
    EXPECT_TRUE(flushResult.has_value());
    EXPECT_EQ(randWritable.getDataAsString(), "Hello, BufferedWriter!");
  }();

  loop->run();
}