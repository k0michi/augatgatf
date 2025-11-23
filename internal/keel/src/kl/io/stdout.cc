#include "kl/io/stdout.hh"

#include <cstring>
#include <vector>

#include "kl/io/awaiter.hh"

namespace kl::io {
Stdout::~Stdout() = default;

kl::concurrent::Task<Expected<std::size_t>>
Stdout::write(std::shared_ptr<Loop> loop, std::span<const std::byte> buffer) {
  auto uvloop = loop ? loop->getUVLoop() : nullptr;

  std::vector<char> temp(buffer.size());
  std::memcpy(temp.data(), buffer.data(), buffer.size());
  WriteAwaiter awaiter{1,
                       uvloop,
                       reinterpret_cast<const std::byte *>(temp.data()),
                       static_cast<unsigned int>(temp.size()),
                       0,
                       {},
                       -1};

  auto nwritten = co_await awaiter;
  co_return nwritten;
}

kl::concurrent::Task<Expected<void>> Stdout::flush(std::shared_ptr<Loop> loop) {
  auto uvloop = loop ? loop->getUVLoop() : nullptr;

  FlushAwaiter awaiter{1, uvloop, {}};
  co_return co_await awaiter;
}
} // namespace kl::io