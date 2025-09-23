#ifndef KL_GRAPHICS_CONTEXT_HH
#define KL_GRAPHICS_CONTEXT_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "context_descriptor.hh"
#include "device_child.hh"

namespace kl::graphics {
class Device;

class Context : public DeviceChild {
private:
  ContextDescriptor mDescriptor;

public:
  virtual ~Context() noexcept = default;

  Context(const Context &) = delete;
  Context(Context &&) noexcept = delete;
  Context &operator=(const Context &) = delete;
  Context &operator=(Context &&) noexcept = delete;

  inline const ContextDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  static std::expected<std::shared_ptr<Context>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const ContextDescriptor &descriptor) noexcept;

private:
  explicit Context(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_CONTEXT_HH
