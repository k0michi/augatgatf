#ifndef KL_TEST_UTIL_HH
#define KL_TEST_UTIL_HH

#include <cstring>
#include <random>

#include "kl/io/async_stream.hh"

class RandomizedAsyncReadable {
public:
  RandomizedAsyncReadable(std::string data, std::uint_fast32_t seed = 42)
      : data_(std::move(data)), offset_(0), rng_(seed) {}

  kl::concurrent::Task<kl::io::Expected<std::size_t>>
  read(std::shared_ptr<kl::io::Loop> loop, std::span<std::byte> buffer) {
    if (offset_ >= data_.size()) {
      co_return 0;
    }

    std::uniform_int_distribution<std::size_t> dist(
        1, std::min(buffer.size(), data_.size() - offset_));

    std::size_t toRead = dist(rng_);
    std::memcpy(buffer.data(), data_.data() + offset_, toRead);
    offset_ += toRead;
    co_return toRead;
  }

private:
  std::string data_;
  std::size_t offset_;
  std::mt19937 rng_;
};

#endif