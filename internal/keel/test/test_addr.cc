#include <gtest/gtest.h>

#include "kl/io/addr.hh"

using namespace kl::io;

TEST(AddrTest, CreateIPv4) {
  auto result = Addr::createIPv4(u8"0.0.0.0", 80);
  ASSERT_TRUE(result.has_value());
  Addr addr = result.value();
  EXPECT_EQ(addr.family(), AddrFamily::eIPv4);
  EXPECT_EQ(addr.ip(), u8"0.0.0.0");
  EXPECT_EQ(addr.port(), 80);
}

TEST(AddrTest, CreateIPv6) {
  auto result = Addr::createIPv6(u8"::1", 80);
  ASSERT_TRUE(result.has_value());
  Addr addr = result.value();
  EXPECT_EQ(addr.family(), AddrFamily::eIPv6);
  EXPECT_EQ(addr.ip(), u8"::1");
  EXPECT_EQ(addr.port(), 80);
}

TEST(AddrTest, CreateInvalidIPv4) {
  auto result = Addr::createIPv4(u8"999.999.999.999", 80);
  ASSERT_FALSE(result.has_value());
}

TEST(AddrTest, CreateInvalidIPv6) {
  auto result = Addr::createIPv6(u8"gggg::gggg", 80);
  ASSERT_FALSE(result.has_value());
}