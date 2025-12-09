#ifndef INTL_HH
#define INTL_HH

#include <cstdint>
#include <expected>
#include <stdexcept>
#include <string>
#include <string_view>

namespace intl_cpp {
std::expected<std::u8string, std::runtime_error>
toUpperCase(std::u8string_view str);

std::expected<std::u8string, std::runtime_error>
toLocaleUpperCase(std::u8string_view str, std::u8string_view locale);

std::expected<std::u8string, std::runtime_error>
toLowerCase(std::u8string_view str);

std::expected<std::u8string, std::runtime_error>
toLocaleLowerCase(std::u8string_view str, std::u8string_view locale);

enum class NormalizationForm { eNFC, eNFD, eNFKC, eNFKD };

std::expected<std::u8string, std::runtime_error>
normalize(std::u8string_view str, NormalizationForm form);

bool isNormalized(std::u8string_view str, NormalizationForm form);

std::expected<std::u8string, std::runtime_error>
toWellFormed(std::u8string_view str);

bool isWellFormed(std::u8string_view str);

std::expected<std::u8string, std::runtime_error> trim(std::u8string_view str);

std::expected<std::u8string, std::runtime_error>
trimStart(std::u8string_view str);

std::expected<std::u8string, std::runtime_error>
trimEnd(std::u8string_view str);

std::expected<std::u8string, std::runtime_error>
padStart(std::u8string_view str, std::size_t maxLength,
         std::u8string_view fillString = u8" ");

std::expected<std::u8string, std::runtime_error>
padEnd(std::u8string_view str, std::size_t maxLength,
       std::u8string_view fillString = u8" ");
} // namespace intl_cpp
#endif