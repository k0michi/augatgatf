#include "kl/io/stderr.hh"

#include <cstring>
#include <vector>

#include "kl/io/awaiter.hh"

namespace kl::io {
Stderr::~Stderr() = default;

kl::concurrent::Task<Expected<std::size_t>>
Stderr::write(std::shared_ptr<Loop> loop, std::span<const std::byte> buffer) {
  auto uvloop = loop ? loop->getUVLoop() : nullptr;

  std::vector<char> temp(buffer.size());
  std::memcpy(temp.data(), buffer.data(), buffer.size());
  WriteAwaiter awaiter{2,
                       uvloop,
                       reinterpret_cast<const std::byte *>(temp.data()),
                       static_cast<unsigned int>(temp.size()),
                       0,
                       {},
                       -1};

  auto nwritten = co_await awaiter;
  co_return nwritten;
}

kl::concurrent::Task<Expected<void>> Stderr::flush(std::shared_ptr<Loop> loop) {
  auto uvloop = loop ? loop->getUVLoop() : nullptr;

  FlushAwaiter awaiter{2, uvloop, {}};
  co_return co_await awaiter;
}
} // namespace kl::io