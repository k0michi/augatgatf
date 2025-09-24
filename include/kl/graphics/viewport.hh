#ifndef KL_GRAPHICS_VIEWPORT_HH
#define KL_GRAPHICS_VIEWPORT_HH

namespace kl::graphics {
struct Viewport final {
  float x = 0;
  float y = 0;
  float width = 0;
  float height = 0;
  float minDepth = 0;
  float maxDepth = 0;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_VIEWPORT_HH
