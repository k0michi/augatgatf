#include "kl/io/stdin.hh"

#include "kl/io/awaiter.hh"

namespace kl::io {
Stdin::~Stdin() = default;

kl::concurrent::Task<Expected<std::size_t>>
Stdin::read(std::shared_ptr<Loop> loop, std::span<std::byte> buffer) {
  auto uvloop = loop ? loop->getUVLoop() : nullptr;
  ReadAwaiter awaiter{
      0,  uvloop, buffer.data(), static_cast<unsigned int>(buffer.size()), 0,
      {}, -1};
  auto nread = co_await awaiter;
  co_return nread;
}
} // namespace kl::io