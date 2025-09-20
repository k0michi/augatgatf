#ifndef KL_PLATFORM_FRAME_AWAITER_HH
#define KL_PLATFORM_FRAME_AWAITER_HH

#include <coroutine>

namespace kl::platform {
class FrameAwaiter {
private:
  std::coroutine_handle<> m_handle;
  double m_time = 0.0;

public:
  bool await_ready() const noexcept;

  void await_suspend(std::coroutine_handle<> handle) noexcept;

  double await_resume() const noexcept;
};
} // namespace kl::platform
#endif // KL_PLATFORM_FRAME_AWAITER_HH
