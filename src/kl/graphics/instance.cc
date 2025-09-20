#include "kl/graphics/instance.hh"

namespace kl::graphics {
std::weak_ptr<Instance> Instance::sInstance;

std::expected<std::shared_ptr<Instance>, std::runtime_error>
Instance::create(const InstanceDescriptor &descriptor) noexcept {
  if (!sInstance.expired()) {
    return std::unexpected(std::runtime_error("Instance already created"));
  }

  auto instance = std::shared_ptr<Instance>(new Instance());
  sInstance = instance;
  return instance;
}
} // namespace kl::graphics
