#ifndef KL_GRAPHICS_OPENGL_GL_CONTEXT_DESCRIPTOR_HH
#define KL_GRAPHICS_OPENGL_GL_CONTEXT_DESCRIPTOR_HH

#include <memory>

#include <SDL3/SDL_video.h>

namespace kl::graphics::opengl {
class GLContext;

struct GLContextDescriptor final {
  SDL_Window *window;
  std::shared_ptr<GLContext> shareContext = nullptr;
  SDL_GLProfile profile = SDL_GL_CONTEXT_PROFILE_CORE;
  uint32_t majorVersion = 3;
  uint32_t minorVersion = 3;
};
} // namespace kl::graphics::opengl
#endif // KL_GRAPHICS_OPENGL_GL_CONTEXT_DESCRIPTOR_HH
