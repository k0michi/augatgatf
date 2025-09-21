#ifndef KL_GRAPHICS_CONTEXT_HH
#define KL_GRAPHICS_CONTEXT_HH

namespace kl::graphics {
class Context {
public:
  virtual ~Context() noexcept = default;

  Context(const Context &) = delete;
  Context(Context &&) noexcept = delete;
  Context &operator=(const Context &) = delete;
  Context &operator=(Context &&) noexcept = delete;

private:
  explicit Context() noexcept = default;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_CONTEXT_HH
