#ifndef KL_GRAPHICS_SHADER_DESCRIPTOR_HH
#define KL_GRAPHICS_SHADER_DESCRIPTOR_HH

#include <cstddef>
#include <variant>
#include <vector>

namespace kl::graphics {
enum class ShaderStage {
  eVertex,
  eFragment,
  // eCompute,
};

struct SpecializationConstantDescriptor final {
  std::uint32_t id;
  std::variant<int32_t, uint32_t, float, double, bool> value;
};

struct ShaderDescriptor final {
  ShaderStage stage = ShaderStage::eVertex;
  std::vector<std::byte> code = {};
  std::string entryPoint = "main";
  std::vector<SpecializationConstantDescriptor> specializationConstants = {};
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SHADER_DESCRIPTOR_HH
