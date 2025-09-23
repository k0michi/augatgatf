#include "kl/text/encoding.hh"
#include <cstddef>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(EncodingTest, EncodeUTF8ToUTF16) {
  std::u8string asciiStr = u8"Hello, World!";
  auto result = kl::text::Encoding::encodeUTF8ToUTF16(asciiStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedAscii = u"Hello, World!";
  EXPECT_EQ(result.value(), expectedAscii);

  std::u8string multiByteStr = u8"こんにちは";
  result = kl::text::Encoding::encodeUTF8ToUTF16(multiByteStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedMultiByte = u"こんにちは";
  EXPECT_EQ(result.value(), expectedMultiByte);

  std::u8string invalidStr = u8"\xFF\xFF\xFF";
  result = kl::text::Encoding::encodeUTF8ToUTF16(invalidStr);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error().what(), std::string("Invalid UTF-8 sequence"));

  std::u8string emojiStr = u8"😃";
  result = kl::text::Encoding::encodeUTF8ToUTF16(emojiStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedEmoji = u"😃";
  EXPECT_EQ(result.value(), expectedEmoji);
}

TEST(EncodingTest, EncodeUTF8ToUTF32) {
  std::u8string asciiStr = u8"Hello, World!";
  auto result = kl::text::Encoding::encodeUTF8ToUTF32(asciiStr);
  ASSERT_TRUE(result.has_value());
  std::u32string expectedAscii = U"Hello, World!";
  EXPECT_EQ(result.value(), expectedAscii);

  std::u8string multiByteStr = u8"こんにちは";
  result = kl::text::Encoding::encodeUTF8ToUTF32(multiByteStr);
  ASSERT_TRUE(result.has_value());
  std::u32string expectedMultiByte = U"こんにちは";
  EXPECT_EQ(result.value(), expectedMultiByte);

  std::u8string invalidStr = u8"\xFF\xFF\xFF";
  result = kl::text::Encoding::encodeUTF8ToUTF32(invalidStr);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error().what(), std::string("Invalid UTF-8 sequence"));

  std::u8string emojiStr = u8"😃";
  result = kl::text::Encoding::encodeUTF8ToUTF32(emojiStr);
  ASSERT_TRUE(result.has_value());
  std::u32string expectedEmoji = U"😃";
  EXPECT_EQ(result.value(), expectedEmoji);
}

TEST(EncodingTest, EncodeUTF32ToUTF16) {
  std::u32string asciiStr = U"Hello, World!";
  auto result = kl::text::Encoding::encodeUTF32ToUTF16(asciiStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedAscii = u"Hello, World!";
  EXPECT_EQ(result.value(), expectedAscii);

  std::u32string multiByteStr = U"こんにちは";
  result = kl::text::Encoding::encodeUTF32ToUTF16(multiByteStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedMultiByte = u"こんにちは";
  EXPECT_EQ(result.value(), expectedMultiByte);

  std::u32string emojiStr = U"😃";
  result = kl::text::Encoding::encodeUTF32ToUTF16(emojiStr);
  ASSERT_TRUE(result.has_value());
  std::u16string expectedEmojiByte = u"😃";
  EXPECT_EQ(result.value(), expectedEmojiByte);
}