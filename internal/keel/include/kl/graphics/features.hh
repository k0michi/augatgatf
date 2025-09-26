#ifndef KL_GRAPHICS_FEATURES_HH
#define KL_GRAPHICS_FEATURES_HH

namespace kl::graphics {
struct Features final {
  bool anisotropicFiltering;
  bool depthClamp;
  bool polygonMode;
  bool independentBlend;
  bool computeShaders;
  // TODO: Add more features.
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_FEATURES_HH
