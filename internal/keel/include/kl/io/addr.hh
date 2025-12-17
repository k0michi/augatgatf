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

  static Expected<Addr> fromPosixAddr(const PosixAddr *addr) {
    if (addr->sa_family == AF_INET) {
      auto inAddr = reinterpret_cast<const struct sockaddr_in *>(addr);
      char ip[INET_ADDRSTRLEN];
      uv_ip4_name(inAddr, ip, sizeof(ip));
      return createIPv4(std::u8string(reinterpret_cast<const char8_t *>(ip)),
                        ntohs(inAddr->sin_port));
    } else if (addr->sa_family == AF_INET6) {
      auto in6Addr = reinterpret_cast<const struct sockaddr_in6 *>(addr);
      char ip[INET6_ADDRSTRLEN];
      uv_ip6_name(in6Addr, ip, sizeof(ip));
      return createIPv6(std::u8string(reinterpret_cast<const char8_t *>(ip)),
                        ntohs(in6Addr->sin6_port));
    } else {
      return std::unexpected(IOException("Unsupported address family"));
    }
  }

  AddrFamily family() const { return family_; }

  std::u8string_view ip() const { return ip_; }

  uint16_t port() const { return port_; }

  const PosixAddr *asPosixAddr() const {
    return reinterpret_cast<const PosixAddr *>(&nativeAddr_);
  }

  Addr() = default;

  Addr(const Addr &other) {
    family_ = other.family_;
    ip_ = other.ip_;
    port_ = other.port_;
    nativeAddr_ = other.nativeAddr_;
  }

  Addr &operator=(const Addr &other) {
    family_ = other.family_;
    ip_ = other.ip_;
    port_ = other.port_;
    nativeAddr_ = other.nativeAddr_;
    return *this;
  }

private:
  AddrFamily family_;
  std::u8string ip_;
  uint16_t port_;
  PosixAddrStorage nativeAddr_;
};
} // namespace kl::io
#endif