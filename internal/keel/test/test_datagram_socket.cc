#include <gtest/gtest.h>

#include "kl/io/datagram_socket.hh"

using namespace kl::io;

TEST(DatagramSocketTest, Create) {
  auto loop = Loop::create();
  ASSERT_NE(loop, nullptr);

  auto result = DatagramSocket::create(loop);
  ASSERT_TRUE(result.has_value());
  auto datagram = result.value();
  ASSERT_NE(datagram, nullptr);
}

TEST(DatagramSocketTest, Bind) {
  auto loop = Loop::create();
  ASSERT_NE(loop, nullptr);

  auto datagramResult = DatagramSocket::create(loop);
  ASSERT_TRUE(datagramResult.has_value());
  auto datagram = datagramResult.value();
  ASSERT_NE(datagram, nullptr);

  auto addrResult = Addr::createIPv4(u8"0.0.0.0", 0);
  ASSERT_TRUE(addrResult.has_value());
  auto addr = addrResult.value();
  auto bindResult = datagram->bind(addr);
  ASSERT_TRUE(bindResult.has_value());
}