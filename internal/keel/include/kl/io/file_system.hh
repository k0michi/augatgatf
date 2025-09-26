#ifndef KL_IO_FILE_SYSTEM_HH
#define KL_IO_FILE_SYSTEM_HH

#include "kl/platform/task.hh"

namespace kl::io {
class FileSystem {
public:
  FileSystem() = delete;

    static kl::platform::Task<std::vector<std::byte>> readFileBinaryAsync(std::string_view filename) {
        co_return co_await loadAsync(filename
};
} // namespace kl::io
#endif // KL_IO_FILE_SYSTEM_HH
