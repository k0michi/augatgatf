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

TEST(IntlString, ToLocaleUpperCase) {
  std::u8string input = u8"iı";
  std::u8string locale = u8"tr";
  auto result = toLocaleUpperCase(input, locale);
  ASSERT_TRUE(result.has_value());
  std::u8string expected = u8"İI";
  EXPECT_EQ(result.value(), expected);

  locale = u8"en";
  result = toLocaleUpperCase(input, locale);
  ASSERT_TRUE(result.has_value());
  expected = u8"II";
  EXPECT_EQ(result.value(), expected);
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

TEST(IntlString, Normalize) {
  std::u8string input = u8"Café";
  auto result = normalize(input, NormalizationForm::eNFC);
  ASSERT_TRUE(result.has_value());
  std::u8string expected = u8"Café";
  EXPECT_EQ(result.value(), expected);

  input = u8"Cafe\u0301"; // 'e' + combining acute accent
  result = normalize(input, NormalizationForm::eNFC);
  ASSERT_TRUE(result.has_value());
  expected = u8"Café";
  EXPECT_EQ(result.value(), expected);

  result = normalize(input, NormalizationForm::eNFD);
  ASSERT_TRUE(result.has_value());
  expected = u8"Cafe\u0301";
  EXPECT_EQ(result.value(), expected);
}