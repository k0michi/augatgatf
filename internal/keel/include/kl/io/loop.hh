#ifndef KL_IO_LOOP_HH
#define KL_IO_LOOP_HH

#include <memory>

#include <uv.h>

namespace kl::io {
class Loop : public std::enable_shared_from_this<Loop> {
public:
  static std::shared_ptr<Loop> create();

  ~Loop() noexcept;

  Loop(const Loop &) = delete;
  Loop &operator=(const Loop &) = delete;

  uv_loop_t *getUVLoop() noexcept { return loop_; }
  const uv_loop_t *getUVLoop() const noexcept { return loop_; }

  static std::shared_ptr<Loop> getDefault();

private:
  uv_loop_t *loop_ = nullptr;
  Loop() = default;
};
} // namespace kl::io
#endif