#ifndef KL_GRAPHICS_INSTANCE_HH
#define KL_GRAPHICS_INSTANCE_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "kl/graphics/instance_descriptor.hh"
#include "surface.hh"

namespace kl::graphics {
class Instance {
private:
  static std::weak_ptr<Instance> sInstance;

public:
  virtual ~Instance() noexcept = default;

  Instance(const Instance &) = delete;
  Instance &operator=(const Instance &) = delete;

  std::expected<std::shared_ptr<Surface>, std::runtime_error>
  createSurface(const SurfaceDescriptor &descriptor) noexcept;

  static std::expected<std::shared_ptr<Instance>, std::runtime_error>
  create(const InstanceDescriptor &descriptor) noexcept;
  static inline std::weak_ptr<Instance> get() noexcept { return sInstance; }

private:
  explicit Instance() noexcept = default;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_INSTANCE_HH
