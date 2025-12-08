#include "intl.hh"

#include <unicode/ucasemap.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>

namespace intl_cpp {
std::u8string toUpperCase(std::u8string_view str) {
  UErrorCode status = U_ZERO_ERROR;
  UCaseMap *caseMap = ucasemap_open(nullptr, 0, &status);

  if (U_FAILURE(status)) {
    return u8"";
  }

  std::int32_t destCapacity = ucasemap_utf8ToUpper(
      caseMap, nullptr, 0, reinterpret_cast<const char *>(str.data()),
      static_cast<std::int32_t>(str.size()), &status);

  if (status != U_BUFFER_OVERFLOW_ERROR && U_FAILURE(status)) {
    ucasemap_close(caseMap);
    return u8"";
  }

  status = U_ZERO_ERROR;
  std::u8string result;
  result.resize(destCapacity);
  ucasemap_utf8ToUpper(caseMap, reinterpret_cast<char *>(result.data()),
                       destCapacity, reinterpret_cast<const char *>(str.data()),
                       static_cast<std::int32_t>(str.size()), &status);
  ucasemap_close(caseMap);

  if (U_FAILURE(status)) {
    return u8"";
  }

  return result;
}

std::expected<std::u8string, std::runtime_error>
toLocaleUpperCase(std::u8string_view str, std::u8string_view locale) {
  UErrorCode status = U_ZERO_ERROR;
  UCaseMap *caseMap =
      ucasemap_open(reinterpret_cast<const char *>(locale.data()), 0, &status);

  if (U_FAILURE(status)) {
    return std::unexpected(std::runtime_error("Failed to open UCaseMap"));
  }

  std::int32_t destCapacity = ucasemap_utf8ToUpper(
      caseMap, nullptr, 0, reinterpret_cast<const char *>(str.data()),
      static_cast<std::int32_t>(str.size()), &status);

  if (status != U_BUFFER_OVERFLOW_ERROR && U_FAILURE(status)) {
    ucasemap_close(caseMap);
    return std::unexpected(
        std::runtime_error("Failed to convert to upper case"));
  }

  status = U_ZERO_ERROR;
  std::u8string result;
  result.resize(destCapacity);
  ucasemap_utf8ToUpper(caseMap, reinterpret_cast<char *>(result.data()),
                       destCapacity, reinterpret_cast<const char *>(str.data()),
                       static_cast<std::int32_t>(str.size()), &status);
  ucasemap_close(caseMap);

  if (U_FAILURE(status)) {
    return std::unexpected(
        std::runtime_error("Failed to convert to upper case"));
  }

  return result;
}

std::u8string toLowerCase(std::u8string_view str) {
  UErrorCode status = U_ZERO_ERROR;
  UCaseMap *caseMap = ucasemap_open(nullptr, 0, &status);

  if (U_FAILURE(status)) {
    return u8"";
  }

  std::int32_t destCapacity = ucasemap_utf8ToLower(
      caseMap, nullptr, 0, reinterpret_cast<const char *>(str.data()),
      static_cast<std::int32_t>(str.size()), &status);

  if (status != U_BUFFER_OVERFLOW_ERROR && U_FAILURE(status)) {
    ucasemap_close(caseMap);
    return u8"";
  }

  status = U_ZERO_ERROR;
  std::u8string result;
  result.resize(destCapacity);
  ucasemap_utf8ToLower(caseMap, reinterpret_cast<char *>(result.data()),
                       destCapacity, reinterpret_cast<const char *>(str.data()),
                       static_cast<std::int32_t>(str.size()), &status);
  ucasemap_close(caseMap);

  if (U_FAILURE(status)) {
    return u8"";
  }

  return result;
}
} // namespace intl_cpp