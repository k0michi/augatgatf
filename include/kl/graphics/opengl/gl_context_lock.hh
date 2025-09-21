#ifndef KL_GRAPHICS_OPENGL_GL_CONTEXT_LOCK_HH
#define KL_GRAPHICS_OPENGL_GL_CONTEXT_LOCK_HH

#include "gl_context.hh"

namespace kl::graphics::opengl {
class GLContextLock {
private:
  std::shared_ptr<GLContext> mContext;

public:
  explicit GLContextLock(std::shared_ptr<GLContext> context) noexcept;
  virtual ~GLContextLock() noexcept;

  GLContextLock(const GLContextLock &) = delete;
  GLContextLock(GLContextLock &&) noexcept = delete;
  GLContextLock &operator=(const GLContextLock &) = delete;
  GLContextLock &operator=(GLContextLock &&) noexcept = delete;
};
} // namespace kl::graphics::opengl
#endif // KL_GRAPHICS_OPENGL_GL_CONTEXT_LOCK_HH
