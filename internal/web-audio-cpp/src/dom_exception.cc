#include "web_audio/dom_exception.h"

namespace web_audio {
DOMException::DOMException(const std::string &message, const std::string &name)
    : message_(message), name_(name) {}

const char *DOMException::what() const noexcept { return message_.c_str(); }

const char *DOMException::getMessage() const { return message_.c_str(); }

const char *DOMException::getName() const { return name_.c_str(); }

std::uint16_t DOMException::getCode() const {
  if (name_ == "IndexSizeError") {
    return INDEX_SIZE_ERR;
  } else if (name_ == "DOMStringSizeError") {
    return DOMSTRING_SIZE_ERR;
  } else if (name_ == "HierarchyRequestError") {
    return HIERARCHY_REQUEST_ERR;
  } else if (name_ == "WrongDocumentError") {
    return WRONG_DOCUMENT_ERR;
  } else if (name_ == "InvalidCharacterError") {
    return INVALID_CHARACTER_ERR;
  } else if (name_ == "NoDataAllowedError") {
    return NO_DATA_ALLOWED_ERR;
  } else if (name_ == "NoModificationAllowedError") {
    return NO_MODIFICATION_ALLOWED_ERR;
  } else if (name_ == "NotFoundError") {
    return NOT_FOUND_ERR;
  } else if (name_ == "NotSupportedError") {
    return NOT_SUPPORTED_ERR;
  } else if (name_ == "InUseAttributeError") {
    return INUSE_ATTRIBUTE_ERR;
  } else if (name_ == "InvalidStateError") {
    return INVALID_STATE_ERR;
  } else if (name_ == "SyntaxError") {
    return SYNTAX_ERR;
  } else if (name_ == "InvalidModificationError") {
    return INVALID_MODIFICATION_ERR;
  } else if (name_ == "NamespaceError") {
    return NAMESPACE_ERR;
  } else if (name_ == "InvalidAccessError") {
    return INVALID_ACCESS_ERR;
  } else if (name_ == "ValidationError") {
    return VALIDATION_ERR;
  } else if (name_ == "TypeMismatchError") {
    return TYPE_MISMATCH_ERR;
  } else if (name_ == "SecurityError") {
    return SECURITY_ERR;
  } else if (name_ == "NetworkError") {
    return NETWORK_ERR;
  } else if (name_ == "AbortError") {
    return ABORT_ERR;
  } else if (name_ == "URLMismatchError") {
    return URL_MISMATCH_ERR;
  } else if (name_ == "QuotaExceededError") {
    return QUOTA_EXCEEDED_ERR;
  } else if (name_ == "TimeoutError") {
    return TIMEOUT_ERR;
  } else if (name_ == "InvalidNodeTypeError") {
    return INVALID_NODE_TYPE_ERR;
  } else if (name_ == "DataCloneError") {
    return DATA_CLONE_ERR;
  } else {
    return 0;
  }
}
} // namespace web_audio