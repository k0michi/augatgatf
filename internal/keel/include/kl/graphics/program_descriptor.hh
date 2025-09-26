#ifndef KL_GRAPHICS_PROGRAM_DESCRIPTOR_HH
#define KL_GRAPHICS_PROGRAM_DESCRIPTOR_HH

#include <memory>
#include <vector>

namespace kl::graphics {
class Shader;

struct ProgramDescriptor final {
  std::vector<std::shared_ptr<Shader>> shaders = {};
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_PROGRAM_DESCRIPTOR_HH
