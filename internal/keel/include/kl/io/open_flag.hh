#ifndef KL_IO_OPEN_FLAG_HH
#define KL_IO_OPEN_FLAG_HH

#include <type_traits>

#ifndef __EMSCRIPTEN__
#include <uv.h>
#endif

namespace kl::io {
#ifndef __EMSCRIPTEN__
enum class OpenFlag {
  eAppend = UV_FS_O_APPEND,
  eCreat = UV_FS_O_CREAT,
  eDirect = UV_FS_O_DIRECT,
  eDirectory = UV_FS_O_DIRECTORY,
  eDsync = UV_FS_O_DSYNC,
  eExcl = UV_FS_O_EXCL,
  eNoAtime = UV_FS_O_NOATIME,
  eNoCTTY = UV_FS_O_NOCTTY,
  eNoFollow = UV_FS_O_NOFOLLOW,
  eNonBlock = UV_FS_O_NONBLOCK,
  eRdOnly = UV_FS_O_RDONLY,
  eRdWr = UV_FS_O_RDWR,
  eSync = UV_FS_O_SYNC,
  eTrunc = UV_FS_O_TRUNC,
  eWrOnly = UV_FS_O_WRONLY,
  eFileMap = UV_FS_O_FILEMAP,
  eRandom = UV_FS_O_RANDOM,
  eShortLived = UV_FS_O_SHORT_LIVED,
  eSequential = UV_FS_O_SEQUENTIAL,
  eTemporary = UV_FS_O_TEMPORARY,
};
#else
enum class OpenFlag {
  eAppend = 0,
  eCreat = 0,
  eDirect = 0,
  eDirectory = 0,
  eDsync = 0,
  eExcl = 0,
  eNoAtime = 0,
  eNoCTTY = 0,
  eNoFollow = 0,
  eNonBlock = 0,
  eRdOnly = 0,
  eRdWr = 0,
  eSync = 0,
  eTrunc = 0,
  eWrOnly = 0,
  eFileMap = 0,
  eRandom = 0,
  eShortLived = 0,
  eSequential = 0,
  eTemporary = 0,
};
#endif

constexpr OpenFlag operator|=(OpenFlag &lhs, OpenFlag rhs) {
  using UT = std::underlying_type_t<OpenFlag>;
  lhs = static_cast<OpenFlag>(static_cast<UT>(lhs) | static_cast<UT>(rhs));
  return lhs;
}

constexpr OpenFlag operator&=(OpenFlag &lhs, OpenFlag rhs) {
  using UT = std::underlying_type_t<OpenFlag>;
  lhs = static_cast<OpenFlag>(static_cast<UT>(lhs) & static_cast<UT>(rhs));
  return lhs;
}

constexpr OpenFlag operator^=(OpenFlag &lhs, OpenFlag rhs) {
  using UT = std::underlying_type_t<OpenFlag>;
  lhs = static_cast<OpenFlag>(static_cast<UT>(lhs) ^ static_cast<UT>(rhs));
  return lhs;
}

constexpr OpenFlag operator|(OpenFlag lhs, OpenFlag rhs) { return lhs |= rhs; }

constexpr OpenFlag operator&(OpenFlag lhs, OpenFlag rhs) { return lhs &= rhs; }

constexpr OpenFlag operator^(OpenFlag lhs, OpenFlag rhs) { return lhs ^= rhs; }

constexpr OpenFlag operator~(OpenFlag val) {
  using UT = std::underlying_type_t<OpenFlag>;
  return static_cast<OpenFlag>(~static_cast<UT>(val));
}
} // namespace kl::io
#endif