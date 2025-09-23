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
}