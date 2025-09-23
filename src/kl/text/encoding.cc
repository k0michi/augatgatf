#include "kl/text/encoding.hh"

namespace kl::text {
std::expected<std::u32string, std::runtime_error>
Encoding::encodeUTF8ToUTF32(std::u8string_view str) noexcept {
  std::vector<char32_t> result;
  size_t i = 0;

  for (size_t j = 0; j < str.size(); ++j) {
    char8_t c = str[j];

    if ((c & 0b1000'0000) == 0) {
      // 1-byte sequence
      result.push_back(static_cast<char32_t>(c));
      i += 1;
    } else if ((c & 0b1110'0000) == 0b1100'0000) {
      // 2-byte sequence
      if (j + 1 >= str.size()) {
        return std::unexpected(std::runtime_error("Invalid UTF-8 sequence"));
      }

      char8_t c1 = str[j + 1];

      if ((c1 & 0b1100'0000) != 0b1000'0000) {
        return std::unexpected(std::runtime_error("Invalid UTF-8 sequence"));
      }

      char32_t codepoint = ((static_cast<char32_t>(c & 0b0001'1111)) << 6) |
                           (static_cast<char32_t>(c1 & 0b0011'1111));
      result.push_back(codepoint);
      j += 1;
      i += 2;
    } else if ((c & 0b1111'0000) == 0b1110'0000) {
      // 3-byte sequence
      if (j + 2 >= str.size()) {
        return std::unexpected(std::runtime_error("Invalid UTF-8 sequence"));
      }

      char8_t c1 = str[j + 1];
      char8_t c2 = str[j + 2];

      if ((c1 & 0b1100'0000) != 0b1000'0000 ||
          (c2 & 0b1100'0000) != 0b1000'0000) {
        return std::unexpected(std::runtime_error("Invalid UTF-8 sequence"));
      }

      char32_t codepoint = ((static_cast<char32_t>(c & 0b0000'1111)) << 12) |
                           ((static_cast<char32_t>(c1 & 0b0011'1111)) << 6) |
                           (static_cast<char32_t>(c2 & 0b0011'1111));
      result.push_back(codepoint);
      j += 2;
      i += 3;
    } else if ((c & 0b1111'1000) == 0b1111'0000) {
      // 4-byte sequence
      if (j + 3 >= str.size()) {
        return std::unexpected(std::runtime_error("Invalid UTF-8 sequence"));
      }

      char8_t c1 = str[j + 1];
      char8_t c2 = str[j + 2];
      char8_t c3 = str[j + 3];

      if ((c1 & 0b1100'0000) != 0b1000'0000 ||
          (c2 & 0b1100'0000) != 0b1000'0000 ||
          (c3 & 0b1100'0000) != 0b1000'0000) {
        return std::unexpected(std::runtime_error("Invalid UTF-8 sequence"));
      }

      char32_t codepoint = ((static_cast<char32_t>(c & 0b0000'0111)) << 18) |
                           ((static_cast<char32_t>(c1 & 0b0011'1111)) << 12) |
                           ((static_cast<char32_t>(c2 & 0b0011'1111)) << 6) |
                           (static_cast<char32_t>(c3 & 0b0011'1111));
      result.push_back(codepoint);
      j += 3;
      i += 4;
    } else {
      return std::unexpected(std::runtime_error("Invalid UTF-8 sequence"));
    }
  }

  return std::u32string(result.begin(), result.end());
}
} // namespace kl::text
