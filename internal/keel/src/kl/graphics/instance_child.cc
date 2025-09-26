#include "kl/graphics/instance_child.hh"

namespace kl::graphics {
InstanceChild::InstanceChild(std::shared_ptr<Instance> instance) noexcept
    : mInstance(instance) {}
} // namespace kl::graphics
