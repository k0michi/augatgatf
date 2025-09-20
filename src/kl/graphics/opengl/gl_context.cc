#include "kl/graphics/opengl/gl_context.hh"

namespace kl::graphics::opengl {
GLContext::~GLContext() {
  if (mContext) {
    SDL_GL_DestroyContext(mContext);
    mContext = nullptr;
  }
}

GLContext::GLContext(GLContext &&other) {
  mWindow = other.mWindow;
  mContext = other.mContext;
  other.mWindow = nullptr;
  other.mContext = nullptr;
}

GLContext &GLContext::operator=(GLContext &&other) {
  if (this != &other) {
    mWindow = other.mWindow;
    mContext = other.mContext;
    other.mWindow = nullptr;
    other.mContext = nullptr;
  }

  return *this;
}

SDL_Window *GLContext::sdlWindow() const noexcept { return mWindow; }

SDL_GLContext GLContext::sdlContext() const noexcept { return mContext; }

std::recursive_mutex &GLContext::mutex() noexcept { return mMutex; }

std::expected<std::shared_ptr<GLContext>, std::runtime_error>
GLContext::create(SDL_Window *window) noexcept {
  auto sdlContext = SDL_GL_CreateContext(window);

  if (!sdlContext) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  auto context = std::shared_ptr<GLContext>(new GLContext());
  context->mWindow = window;
  context->mContext = sdlContext;
  return context;
}
} // namespace kl::graphics::opengl