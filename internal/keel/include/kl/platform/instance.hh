#ifndef KL_PLATFORM_INSTANCE_HH
#define KL_PLATFORM_INSTANCE_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "kl/concurrent/task.hh"
#include "kl/platform/frame_awaiter.hh"
#include "kl/platform/instance_descriptor.hh"
#include "kl/platform/window.hh"
#include "mouse_state.hh"

namespace kl::platform {
class Instance {
private:
  bool mShouldQuit = false;
  static std::weak_ptr<Instance> sInstance;
  common::Offset2<float> mAccumulatedWheel{0, 0};

public:
  virtual ~Instance() noexcept;

  Instance(const Instance &) = delete;
  Instance &operator=(const Instance &) = delete;

  void pollEvents() noexcept;
  bool shouldQuit() const noexcept;
  FrameAwaiter waitFrame() noexcept;

  std::expected<std::shared_ptr<Window>, std::runtime_error>
  createWindow(const WindowDescriptor &descriptor) noexcept;
  MouseState getMouseState() const noexcept;

  static std::expected<std::shared_ptr<Instance>, std::runtime_error>
  create(const InstanceDescriptor &descriptor) noexcept;
  static inline std::weak_ptr<Instance> get() noexcept { return sInstance; }

protected:
  explicit Instance() noexcept = default;
};
} // namespace kl::platform
#endif // KL_PLATFORM_INSTANCE_HH
