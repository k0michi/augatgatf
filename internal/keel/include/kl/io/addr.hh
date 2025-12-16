#ifndef KL_IO_ADDR_HH
#define KL_IO_ADDR_HH

#include <string>

#include <uv.h>

#include "kl/io/io_exception.hh"

namespace kl::io {
using PosixAddr = struct sockaddr;
using PosixAddrStorage = struct sockaddr_storage;

enum class AddrFamily {
  eIPv4 = AF_INET,
  eIPv6 = AF_INET6,
};

struct Addr {
  static Expected<Addr> createIPv4(const std::u8string &ip, uint16_t port) {
    Addr addr;
    addr.family_ = AddrFamily::eIPv4;
    addr.ip_ = ip;
    addr.port_ = port;
    auto result =
        uv_ip4_addr(reinterpret_cast<const char *>(ip.c_str()), port,
                    reinterpret_cast<struct sockaddr_in *>(&addr.nativeAddr_));

    if (result < 0) {
      return std::unexpected(
          IOException(std::string("Failed to create IPv4 address: ") +
                      uv_strerror(result)));
    }

    return addr;
  }

  static Expected<Addr> createIPv6(const std::u8string &ip, uint16_t port) {
    Addr addr;
    addr.family_ = AddrFamily::eIPv6;
    addr.ip_ = ip;
    addr.port_ = port;

    auto result =
        uv_ip6_addr(reinterpret_cast<const char *>(ip.c_str()), port,
                    reinterpret_cast<struct sockaddr_in6 *>(&addr.nativeAddr_));

    if (result < 0) {
      return std::unexpected(
          IOException(std::string("Failed to create IPv6 address: ") +
                      uv_strerror(result)));
    }

    return addr;
  }

  AddrFamily family() const { return family_; }

  std::u8string_view ip() const { return ip_; }

  uint16_t port() const { return port_; }

  const PosixAddr *posixAddr() const {
    return reinterpret_cast<const PosixAddr *>(&nativeAddr_);
  }

private:
  AddrFamily family_;
  std::u8string ip_;
  uint16_t port_;
  PosixAddrStorage nativeAddr_;
};
} // namespace kl::io
#endif