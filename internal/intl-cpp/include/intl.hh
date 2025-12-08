#ifndef INTL_HH
#define INTL_HH

#include <cstdint>
#include <expected>
#include <stdexcept>
#include <string>
#include <string_view>

namespace intl_cpp {
std::u8string toUpperCase(std::u8string_view str);

std::expected<std::u8string, std::runtime_error>
toLocaleUpperCase(std::u8string_view str, std::u8string_view locale);

std::u8string toLowerCase(std::u8string_view str);

std::expected<std::u8string, std::runtime_error>
toLocaleLowerCase(std::u8string_view str, std::u8string_view locale);

enum class NormalizationForm { eNFC, eNFD, eNFKC, eNFKD };

std::expected<std::u8string, std::runtime_error>
normalize(std::u8string_view str, NormalizationForm form);
} // namespace intl_cpp
#endif