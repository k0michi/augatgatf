#include "kl/graphics/context.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<Context>, std::runtime_error>
Context::create(std::shared_ptr<Device> device,
                const ContextDescriptor &descriptor) noexcept {
  auto context = std::shared_ptr<Context>(new Context());
  return context;
}
} // namespace kl::graphics
