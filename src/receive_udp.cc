#include <kl/io/datagram_socket.hh>

#include <iostream>

int main() {
  auto loop = kl::io::Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto datagramResult = kl::io::DatagramSocket::create(loop);
    if (!datagramResult.has_value()) {
      co_return;
    }
    auto datagram = datagramResult.value();

    auto addrResult = kl::io::Addr::createIPv4(u8"0.0.0.0", 12345);
    if (!addrResult.has_value()) {
      co_return;
    }
    auto addr = addrResult.value();
    auto bindResult = datagram->bind(addr);
    if (!bindResult.has_value()) {
      co_return;
    }
    while (true) {
      auto receiveResult = co_await datagram->receive();
      if (!receiveResult.has_value()) {
        continue;
      }
      auto [data, senderAddr] = receiveResult.value();
      std::string_view message{reinterpret_cast<const char *>(data.data()),
                               data.size()};
      std::cout << message << std::endl;
    }

    co_await datagram->close();
  }();
  loop->run();
  return 0;
}