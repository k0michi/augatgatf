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

TEST(IntlString, ToLocaleLowerCase) {
  std::u8string input = u8"İI";
  std::u8string locale = u8"tr";
  auto result = toLocaleLowerCase(input, locale);
  ASSERT_TRUE(result.has_value());
  std::u8string expected = u8"iı";
  EXPECT_EQ(result.value(), expected);

  locale = u8"en";
  result = toLocaleLowerCase(input, locale);
  ASSERT_TRUE(result.has_value());
  expected = u8"i̇i";
  EXPECT_EQ(result.value(), expected);
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

TEST(IntlString, IsNormalized) {
  std::u8string input = u8"Café";
  EXPECT_TRUE(isNormalized(input, NormalizationForm::eNFC));
  EXPECT_FALSE(isNormalized(input, NormalizationForm::eNFD));

  input = u8"Cafe\u0301"; // 'e' + combining acute accent
  EXPECT_FALSE(isNormalized(input, NormalizationForm::eNFC));
  EXPECT_TRUE(isNormalized(input, NormalizationForm::eNFD));
}

TEST(IntlString, ToWellFormed) {
  std::u8string input = u8"Hello, World!";
  auto result = toWellFormed(input);
  ASSERT_TRUE(result.has_value());
  std::u8string expected = u8"Hello, World!";
  EXPECT_EQ(result.value(), expected);

  input = std::u8string{0xC3, 0x28}; // Invalid 2-byte sequence
  result = toWellFormed(input);
  ASSERT_TRUE(result.has_value());
  expected = std::u8string{0xEF, 0xBF, 0xBD, 0x28}; // U+FFFD + '('
  EXPECT_EQ(result.value(), expected);
}

TEST(IntlString, IsWellFormed) {
  std::u8string input = u8"Hello, World!";
  EXPECT_TRUE(isWellFormed(input));

  input = std::u8string{0xC3, 0x28}; // Invalid 2-byte sequence
  EXPECT_FALSE(isWellFormed(input));

  input = std::u8string{0x28, 0xC3}; // Invalid sequence at end
  EXPECT_FALSE(isWellFormed(input));
}

TEST(IntlString, Trim) {
  std::u8string input = u8"\u0020\u00A0\u1680\u2000\u2001\u2002\u2003"
                        u8"\u2004\u2005\u2006\u2007\u2008\u2009\u200A"
                        u8"\u202F\u205F\u3000Hello, World!\u0020\u00A0"
                        u8"\u1680\u2000\u2001\u2002\u2003\u2004\u2005"
                        u8"\u2006\u2007\u2008\u2009\u200A\u202F\u205F"
                        u8"\u3000";
  auto result = trim(input);
  ASSERT_TRUE(result.has_value());
  std::u8string expected = u8"Hello, World!";
  EXPECT_EQ(result.value(), expected);
}

TEST(IntlString, TrimStart) {
  std::u8string input = u8"\u0020\u00A0\u1680\u2000\u2001\u2002\u2003"
                        u8"\u2004\u2005\u2006\u2007\u2008\u2009\u200A"
                        u8"\u202F\u205F\u3000Hello, World!";
  auto result = trimStart(input);
  ASSERT_TRUE(result.has_value());
  std::u8string expected = u8"Hello, World!";
  EXPECT_EQ(result.value(), expected);
}

TEST(IntlString, TrimEnd) {
  std::u8string input = u8"Hello, World!\u0020\u00A0\u1680\u2000\u2001"
                        u8"\u2002\u2003\u2004\u2005\u2006\u2007\u2008"
                        u8"\u2009\u200A\u202F\u205F\u3000";
  auto result = trimEnd(input);
  ASSERT_TRUE(result.has_value());
  std::u8string expected = u8"Hello, World!";
  EXPECT_EQ(result.value(), expected);
}