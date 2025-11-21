#include "kl/io/loop.hh"

namespace kl::io {
std::shared_ptr<Loop> Loop::create() {
  std::shared_ptr<Loop> ptr(new Loop());
  ptr->loop_ = new uv_loop_t;
  uv_loop_init(ptr->loop_);
  return ptr;
}

void Loop::run() { uv_run(loop_, UV_RUN_DEFAULT); }

void Loop::poll() { uv_run(loop_, UV_RUN_NOWAIT); }

void Loop::wait() { uv_run(loop_, UV_RUN_ONCE); }

void Loop::stop() { uv_stop(loop_); }

Loop::~Loop() noexcept {
  uv_loop_close(loop_);
  delete loop_;
}

std::shared_ptr<Loop> Loop::getDefault() {
  thread_local std::shared_ptr<Loop> defaultLoop = [] {
    auto ptr = std::shared_ptr<Loop>(new Loop());
    ptr->loop_ = new uv_loop_t;
    uv_loop_init(ptr->loop_);
    return ptr;
  }();
  return defaultLoop;
}
} // namespace kl::io
