#include "kl/version.hh"

namespace kl {
Version getVersion() noexcept { return kHeaderVersion; }

std::string_view getRevision() noexcept { return kRevision; }
} // namespace kl