#include "kl/graphics/opengl/gl_context_lock.hh"

namespace kl::graphics::opengl {
GLContextLock::GLContextLock(std::shared_ptr<GLContext> context) noexcept
    : mContext(std::move(context)) {
  mContext->mutex().lock();
  SDL_GL_MakeCurrent(mContext->sdlWindow(), mContext->sdlContext());
}

GLContextLock::~GLContextLock() noexcept {
  SDL_GL_MakeCurrent(nullptr, nullptr);
  mContext->mutex().unlock();
}
} // namespace kl::graphics::opengl
