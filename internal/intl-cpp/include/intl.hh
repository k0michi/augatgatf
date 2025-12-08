#ifndef INTL_HH
#define INTL_HH

#include <cstdint>
#include <string>
#include <string_view>

namespace intl_cpp {
std::u8string toUpperCase(std::u8string_view str);

std::u8string toLowerCase(std::u8string_view str);

} // namespace intl_cpp

#endif