#ifndef KL_GRAPHICS_OPENGL_GL_CONTEXT_HH
#define KL_GRAPHICS_OPENGL_GL_CONTEXT_HH

#include <expected>
#include <memory>
#include <mutex>
#include <stdexcept>

#include <SDL3/SDL_video.h>

namespace kl::graphics::opengl {
class GLContext {
private:
  SDL_Window *mWindow;
  SDL_GLContext mContext;
  std::recursive_mutex mMutex;

public:
  virtual ~GLContext() noexcept = default;

  GLContext(const GLContext &) = delete;
  GLContext(GLContext &&);
  GLContext &operator=(const GLContext &) = delete;
  GLContext &operator=(GLContext &&);

  SDL_Window *sdlWindow() const noexcept;
  SDL_GLContext sdlContext() const noexcept;
  std::recursive_mutex &mutex() noexcept;

  static std::expected<std::shared_ptr<GLContext>, std::runtime_error>
  create(SDL_Window *window) noexcept;

private:
  explicit GLContext() noexcept = default;
};
} // namespace kl::graphics::opengl
#endif // KL_GRAPHICS_OPENGL_GL_CONTEXT_HH
