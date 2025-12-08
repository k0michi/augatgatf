#include "intl.hh"

#include <vector>

#include <unicode/ucasemap.h>
#include <unicode/uloc.h>
#include <unicode/unistr.h>
#include <unicode/unorm2.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>

namespace intl_cpp {
bool isValidLocale(std::u8string_view locale) {
  std::int32_t count = uloc_countAvailable();

  for (std::int32_t i = 0; i < count; ++i) {
    const char *availableLocale = uloc_getAvailable(i);
    if (locale ==
        std::u8string_view(reinterpret_cast<const char8_t *>(availableLocale),
                           std::char_traits<char>::length(availableLocale))) {
      return true;
    }
  }

  return false;
}

std::expected<std::u8string, std::runtime_error>
toCase(std::u8string_view str, std::u8string_view locale, bool toUpper) {
  if (!locale.empty() && !isValidLocale(locale)) {
    return std::unexpected(std::runtime_error("Invalid locale"));
  }

  UErrorCode status = U_ZERO_ERROR;
  UCaseMap *caseMap =
      ucasemap_open(reinterpret_cast<const char *>(locale.data()), 0, &status);
  if (U_FAILURE(status)) {
    return std::unexpected(std::runtime_error("Failed to open UCaseMap"));
  }

  std::int32_t destCapacity =
      toUpper
          ? ucasemap_utf8ToUpper(caseMap, nullptr, 0,
                                 reinterpret_cast<const char *>(str.data()),
                                 static_cast<std::int32_t>(str.size()), &status)
          : ucasemap_utf8ToLower(
                caseMap, nullptr, 0, reinterpret_cast<const char *>(str.data()),
                static_cast<std::int32_t>(str.size()), &status);
  if (status != U_BUFFER_OVERFLOW_ERROR && U_FAILURE(status)) {
    ucasemap_close(caseMap);
    return std::unexpected(std::runtime_error("Failed to convert case"));
  }

  status = U_ZERO_ERROR;
  std::u8string result;
  result.resize(destCapacity);
  if (toUpper) {
    ucasemap_utf8ToUpper(caseMap, reinterpret_cast<char *>(result.data()),
                         destCapacity,
                         reinterpret_cast<const char *>(str.data()),
                         static_cast<std::int32_t>(str.size()), &status);
  } else {
    ucasemap_utf8ToLower(caseMap, reinterpret_cast<char *>(result.data()),
                         destCapacity,
                         reinterpret_cast<const char *>(str.data()),
                         static_cast<std::int32_t>(str.size()), &status);
  }
  ucasemap_close(caseMap);

  if (U_FAILURE(status)) {
    return std::unexpected(std::runtime_error("Failed to convert case"));
  }

  return result;
}

std::expected<std::u8string, std::runtime_error>
toUpperCase(std::u8string_view str) {
  return toCase(str, u8"", true);
}

std::expected<std::u8string, std::runtime_error>
toLocaleUpperCase(std::u8string_view str, std::u8string_view locale) {
  return toCase(str, locale, true);
}

std::expected<std::u8string, std::runtime_error>
toLowerCase(std::u8string_view str) {
  return toCase(str, u8"", false);
}

std::expected<const UNormalizer2 *, std::runtime_error>
getNormalizer(NormalizationForm form) {
  UErrorCode status = U_ZERO_ERROR;
  const UNormalizer2 *norm2 = nullptr;
  switch (form) {
  case NormalizationForm::eNFC:
    norm2 = unorm2_getNFCInstance(&status);
    break;
  case NormalizationForm::eNFD:
    norm2 = unorm2_getNFDInstance(&status);
    break;
  case NormalizationForm::eNFKC:
    norm2 = unorm2_getNFKCInstance(&status);
    break;
  case NormalizationForm::eNFKD:
    norm2 = unorm2_getNFKDInstance(&status);
    break;
  default:
    return std::unexpected(std::runtime_error("Invalid normalization form"));
  }

  if (U_FAILURE(status)) {
    return std::unexpected(std::runtime_error("Failed to get Normalizer2"));
  }

  return norm2;
}

icu::UnicodeString toUnicodeString(std::u8string_view str) {
  return icu::UnicodeString::fromUTF8(
      icu::StringPiece(reinterpret_cast<const char *>(str.data()),
                       static_cast<int32_t>(str.size())));
}

std::u8string toU8String(const icu::UnicodeString &unicodeStr) {
  std::string utf8Str;
  unicodeStr.toUTF8String(utf8Str);
  return std::u8string(reinterpret_cast<const char8_t *>(utf8Str.data()),
                       utf8Str.size());
}

std::expected<std::u8string, std::runtime_error>
normalize(std::u8string_view str, NormalizationForm form) {
  UErrorCode status = U_ZERO_ERROR;
  auto norm2Result = getNormalizer(form);

  if (!norm2Result) {
    return std::unexpected(norm2Result.error());
  }

  auto norm2 = norm2Result.value();
  auto unicodeStr = toUnicodeString(str);

  int32_t destCapacity = unorm2_normalize(
      norm2, unicodeStr.getBuffer(), unicodeStr.length(), nullptr, 0, &status);

  if (status != U_BUFFER_OVERFLOW_ERROR && U_FAILURE(status)) {
    return std::unexpected(
        std::runtime_error(std::string("Normalization failed")));
  }

  status = U_ZERO_ERROR;
  std::vector<UChar> buffer(destCapacity);
  int32_t actualLen =
      unorm2_normalize(norm2, unicodeStr.getBuffer(), unicodeStr.length(),
                       buffer.data(), destCapacity, &status);
  if (U_FAILURE(status)) {
    return std::unexpected(std::runtime_error("Normalization failed"));
  }

  icu::UnicodeString normalizedStr(buffer.data(), actualLen);
  return toU8String(normalizedStr);
}
} // namespace intl_cpp