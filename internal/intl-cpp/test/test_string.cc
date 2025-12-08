#include <gtest/gtest.h>

#include "intl.hh"

using namespace intl_cpp;

TEST(IntlString, ToUpperCase) {
  std::u8string input = u8"Hello, World!";
  std::u8string expected = u8"HELLO, WORLD!";
  EXPECT_EQ(toUpperCase(input), expected);

  input = u8"ßäöü";
  expected = u8"SSÄÖÜ";
  EXPECT_EQ(toUpperCase(input), expected);

  input = u8"íñtérnâtiônålîzâtîön";
  expected = u8"ÍÑTÉRNÂTIÔNÅLÎZÂTÎÖN";
  EXPECT_EQ(toUpperCase(input), expected);
}

TEST(IntlString, ToLowerCase) {
  std::u8string input = u8"Hello, World!";
  std::u8string expected = u8"hello, world!";
  EXPECT_EQ(toLowerCase(input), expected);

  input = u8"ßÄÖÜ";
  expected = u8"ßäöü";
  EXPECT_EQ(toLowerCase(input), expected);

  input = u8"ÍÑTÉRNÂTIÔNÅLÎZÂTÎÖN";
  expected = u8"íñtérnâtiônålîzâtîön";
  EXPECT_EQ(toLowerCase(input), expected);
}