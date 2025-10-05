#pragma once

#include <optional>

namespace web_audio {
// TODO: Inherit from EventInit
struct AudioRenderCapacityEventInit {
  double timestamp = 0;
  double averageLoad = 0;
  double peakLoad = 0;
  double underrunRatio = 0;
};
} // namespace web_audio