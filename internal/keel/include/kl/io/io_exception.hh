#ifndef KL_IO_IO_EXCEPTION_HH
#define KL_IO_IO_EXCEPTION_HH

#include <expected>
#include <stdexcept>
#include <string>

namespace kl::io {
class IOException : public std::runtime_error {
public:
  explicit IOException(const std::string &message)
      : std::runtime_error(message) {}
};

template <typename T> using Expected = std::expected<T, IOException>;
} // namespace kl::io
#endif