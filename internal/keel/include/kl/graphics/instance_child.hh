#ifndef KL_GRAPHICS_INSTANCE_CHILD_HH
#define KL_GRAPHICS_INSTANCE_CHILD_HH

#include <memory>

namespace kl::graphics {
class Instance;

class InstanceChild {
private:
  std::weak_ptr<Instance> mInstance;

public:
  explicit InstanceChild(std::shared_ptr<Instance> instance) noexcept;
  virtual ~InstanceChild() noexcept = default;

  InstanceChild(const InstanceChild &) = delete;
  InstanceChild(InstanceChild &&) noexcept = delete;
  InstanceChild &operator=(const InstanceChild &) = delete;
  InstanceChild &operator=(InstanceChild &&) noexcept = delete;

  std::weak_ptr<Instance> instance() const noexcept { return mInstance; }
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_INSTANCE_CHILD_HH
