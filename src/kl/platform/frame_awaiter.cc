#include "kl/platform/frame_awaiter.hh"

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

namespace kl::platform {
bool FrameAwaiter::await_ready() const noexcept {
#ifdef __EMSCRIPTEN__
  return false;
#else
  return true;
#endif
}

void FrameAwaiter::await_suspend(std::coroutine_handle<> handle) noexcept {
  m_handle = handle;
#ifdef __EMSCRIPTEN__
  emscripten_request_animation_frame(
      [](double time, void *userData) {
        auto *self = static_cast<FrameAwaiter *>(userData);
        self->m_time = time;
        self->m_handle.resume();
        return false;
      },
      this);
#else
#endif
}

double FrameAwaiter::await_resume() const noexcept { return m_time; }
} // namespace kl::platform
