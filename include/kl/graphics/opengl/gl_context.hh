#ifndef KL_GRAPHICS_OPENGL_GL_CONTEXT_HH
#define KL_GRAPHICS_OPENGL_GL_CONTEXT_HH

#include <expected>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include <SDL3/SDL_video.h>
#include <glad/gl.h>

#include "gl_context_descriptor.hh"

namespace kl::graphics::opengl {
/**
 * @brief Internal OpenGL context wrapper.
 */
class GLContext {
private:
  SDL_Window *mWindow;
  SDL_GLContext mContext;
  std::recursive_mutex mMutex;
  std::unique_ptr<GladGLContext> mGladGLContext;
  std::unordered_set<std::string> mExtensions;
  uint32_t mLockCount = 0;

public:
  virtual ~GLContext() noexcept;

  GLContext(const GLContext &) = delete;
  GLContext(GLContext &&);
  GLContext &operator=(const GLContext &) = delete;
  GLContext &operator=(GLContext &&);

  SDL_Window *sdlWindow() const noexcept;
  SDL_GLContext sdlContext() const noexcept;
  std::recursive_mutex &mutex() noexcept;
  GladGLContext *gladGLContext() const noexcept;
  void lock() noexcept;
  void unlock() noexcept;
  bool hasExtension(const std::string &extension) const noexcept;

  static std::expected<std::shared_ptr<GLContext>, std::runtime_error>
  create(const GLContextDescriptor &descriptor) noexcept;

private:
  explicit GLContext() noexcept = default;
};
} // namespace kl::graphics::opengl
#endif // KL_GRAPHICS_OPENGL_GL_CONTEXT_HH
