#include <kl/io/stdout.hh>

int main() {
  auto loop = kl::io::Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    std::string_view message = "Hello, World!\n";
    auto bytes = std::as_bytes(std::span{message.data(), message.size()});
    co_await kl::io::out->write(loop, bytes);
    co_await kl::io::out->flush(loop);
  }();
  loop->run();
  return 0;
}