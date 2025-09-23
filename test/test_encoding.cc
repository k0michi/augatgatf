#include "kl/text/encoding.hh"
#include <cstddef>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(EncodingTest, EncodeUTF8ToUTF32) {
  // Test with a simple ASCII string
  std::u8string_view asciiStr = u8"Hello, World!";
  auto result = kl::text::Encoding::encodeUTF8ToUTF32(asciiStr);
  ASSERT_TRUE(result.has_value());
  std::u32string expectedAscii = U"Hello, World!";
  EXPECT_EQ(result.value(), expectedAscii);

  // Test with a string containing multi-byte UTF-8 characters
  std::u8string_view multiByteStr = u8"こんにちは"; // "Hello" in Japanese
  result = kl::text::Encoding::encodeUTF8ToUTF32(multiByteStr);
  ASSERT_TRUE(result.has_value());
  std::u32string expectedMultiByte = U"こんにちは";
  EXPECT_EQ(result.value(), expectedMultiByte);

  // Test with an invalid UTF-8 sequence
  std::u8string_view invalidStr = u8"\xFF\xFF\xFF"; // Invalid UTF-8
  result = kl::text::Encoding::encodeUTF8ToUTF32(invalidStr);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error().what(), std::string("Invalid UTF-8 sequence"));

  std::u8string_view emojiStr = u8"😃";
  result = kl::text::Encoding::encodeUTF8ToUTF32(emojiStr);
  ASSERT_TRUE(result.has_value());
  std::u32string expectedEmoji = U"😃";
  EXPECT_EQ(result.value(), expectedEmoji);
}

TEST(EncodingTest, EncodeUTF32ToUTF16) {
  // Test with a simple ASCII string
  std::u32string_view asciiStr = U"Hello, World!";
  auto result = kl::text::Encoding::encodeUTF32ToUTF16(asciiStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedAscii = u"Hello, World!";
  EXPECT_EQ(result.value(), expectedAscii);

  // Test with a string containing multi-byte UTF-32 characters
  std::u32string_view multiByteStr = U"こんにちは"; // "Hello" in Japanese
  result = kl::text::Encoding::encodeUTF32ToUTF16(multiByteStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedMultiByte = u"こんにちは";
  EXPECT_EQ(result.value(), expectedMultiByte);

  std::u32string_view emojiStr = U"😃";
  result = kl::text::Encoding::encodeUTF32ToUTF16(multiByteStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedEmojiByte = u"😃";
  EXPECT_EQ(result.value(), expectedEmojiByte);
}