#ifndef KL_TEXT_ENCODING_HH
#define KL_TEXT_ENCODING_HH

#include <cstddef>
#include <expected>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace kl::text {
struct Encoding final {
  /**
   * @brief Encode a UTF-8 string to UTF-16.
   */
  static std::expected<std::u16string, std::runtime_error>
  encodeUTF8ToUTF16(std::u8string_view str) noexcept;

  /**
   * @brief Encode a UTF-8 string to UTF-32.
   */
  static std::expected<std::u32string, std::runtime_error>
  encodeUTF8ToUTF32(std::u8string_view str) noexcept;

  static std::expected<std::u16string, std::runtime_error>
  encodeUTF32ToUTF16(std::u32string_view str) noexcept;
};
} // namespace kl::text
#endif // KL_TEXT_ENCODING_HH
