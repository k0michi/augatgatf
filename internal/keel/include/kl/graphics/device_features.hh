#ifndef KL_GRAPHICS_DEVICE_FEATURES_HH
#define KL_GRAPHICS_DEVICE_FEATURES_HH

namespace kl::graphics {
struct DeviceFeatures final {
  bool anisotropicFiltering;
  bool depthClamp;
  bool polygonMode;
  bool independentBlend;
  bool computeShaders;
  // TODO: Add more features.
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_DEVICE_FEATURES_HH
