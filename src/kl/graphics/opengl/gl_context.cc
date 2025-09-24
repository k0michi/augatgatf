#include "kl/graphics/opengl/gl_context.hh"

namespace kl::graphics::opengl {
GLContext::~GLContext() noexcept {
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

GladGLContext *GLContext::gladGLContext() const noexcept {
  return mGladGLContext.get();
}

void GLContext::lock() noexcept {
  mMutex.lock();
  SDL_GL_MakeCurrent(mWindow, mContext);
}

void GLContext::unlock() noexcept {
  SDL_GL_MakeCurrent(nullptr, nullptr);
  mMutex.unlock();
}

std::expected<std::shared_ptr<GLContext>, std::runtime_error>
GLContext::create(const GLContextDescriptor &descriptor) noexcept {
  auto context = std::shared_ptr<GLContext>(new GLContext());

  if (descriptor.shareContext) {
    SDL_GL_MakeCurrent(descriptor.shareContext->sdlWindow(),
                       descriptor.shareContext->sdlContext());
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  } else {
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, descriptor.majorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, descriptor.minorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, descriptor.profile);

  auto sdlContext = SDL_GL_CreateContext(descriptor.window);

  context->mGladGLContext = std::make_unique<GladGLContext>();
  gladLoadGLContext(context->mGladGLContext.get(), &SDL_GL_GetProcAddress);

  GLint extensionsCount = 0;
  context->mGladGLContext->GetIntegerv(GL_NUM_EXTENSIONS, &extensionsCount);
  for (GLint i = 0; i < extensionsCount; ++i) {
    const char *extension =
        reinterpret_cast<const char *>(context->mGladGLContext->GetStringi(
            GL_EXTENSIONS, static_cast<GLuint>(i)));
    context->mExtensions.insert(extension);
  }

  SDL_GL_MakeCurrent(nullptr, nullptr);

  if (!sdlContext) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  context->mWindow = descriptor.window;
  context->mContext = sdlContext;
  return context;
}
} // namespace kl::graphics::opengl