#pragma once

#include <cstdint>
#include <exception>
#include <string>

#include "details/common.hh"

namespace web_audio {
class DOMException : public std::exception {
  WEB_AUDIO_PRIVATE : std::string message_;
  std::string name_;

public:
  DOMException(const std::string &message, const std::string &name);

  const char *what() const noexcept override;

  const char *getMessage() const;

  const char *getName() const;

  std::uint16_t getCode() const;

  static constexpr std::uint16_t INDEX_SIZE_ERR = 1;
  static constexpr std::uint16_t DOMSTRING_SIZE_ERR = 2;
  static constexpr std::uint16_t HIERARCHY_REQUEST_ERR = 3;
  static constexpr std::uint16_t WRONG_DOCUMENT_ERR = 4;
  static constexpr std::uint16_t INVALID_CHARACTER_ERR = 5;
  static constexpr std::uint16_t NO_DATA_ALLOWED_ERR = 6;
  static constexpr std::uint16_t NO_MODIFICATION_ALLOWED_ERR = 7;
  static constexpr std::uint16_t NOT_FOUND_ERR = 8;
  static constexpr std::uint16_t NOT_SUPPORTED_ERR = 9;
  static constexpr std::uint16_t INUSE_ATTRIBUTE_ERR = 10;
  static constexpr std::uint16_t INVALID_STATE_ERR = 11;
  static constexpr std::uint16_t SYNTAX_ERR = 12;
  static constexpr std::uint16_t INVALID_MODIFICATION_ERR = 13;
  static constexpr std::uint16_t NAMESPACE_ERR = 14;
  static constexpr std::uint16_t INVALID_ACCESS_ERR = 15;
  static constexpr std::uint16_t VALIDATION_ERR = 16;
  static constexpr std::uint16_t TYPE_MISMATCH_ERR = 17;
  static constexpr std::uint16_t SECURITY_ERR = 18;
  static constexpr std::uint16_t NETWORK_ERR = 19;
  static constexpr std::uint16_t ABORT_ERR = 20;
  static constexpr std::uint16_t URL_MISMATCH_ERR = 21;
  static constexpr std::uint16_t QUOTA_EXCEEDED_ERR = 22;
  static constexpr std::uint16_t TIMEOUT_ERR = 23;
  static constexpr std::uint16_t INVALID_NODE_TYPE_ERR = 24;
  static constexpr std::uint16_t DATA_CLONE_ERR = 25;
};
} // namespace web_audio