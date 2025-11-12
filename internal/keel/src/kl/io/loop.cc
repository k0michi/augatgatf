#include "kl/io/loop.hh"

namespace kl::io {
std::shared_ptr<Loop> Loop::create() {
  std::shared_ptr<Loop> ptr(new Loop());
  ptr->loop_ = new uv_loop_t;
  uv_loop_init(ptr->loop_);
  return ptr;
}

Loop::~Loop() noexcept {
  if (loop_) {
    uv_loop_close(loop_);
    delete loop_;
  }
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
