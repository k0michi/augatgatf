#include <kl/io/stdin.hh>
#include <kl/io/stdout.hh>

#include <string_view>
#include <vector>

int main() {
  auto loop = kl::io::Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto buffer = std::vector<std::byte>(1024);
    while (true) {
      auto nread = co_await kl::io::in->read(loop, std::span{buffer});

      if (!nread || *nread == 0) {
        break;
      }

      auto bytes = std::as_bytes(std::span{buffer.data(), *nread});
      co_await kl::io::out->write(loop, bytes);
      co_await kl::io::out->flush(loop);
    }
  }();
  loop->run();
  return 0;
}