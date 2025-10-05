#pragma once

#include <cstdint>

#include "audio_node_options.hh"

namespace web_audio {
struct AnalyzerOptions : public AudioNodeOptions {
  std::uint32_t fftSize = 2048;
  double maxDecibels = -30.0f;
  double minDecibels = -100.0f;
  double smoothingTimeConstant = 0.8;
};
} // namespace web_audio