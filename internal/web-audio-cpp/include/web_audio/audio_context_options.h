#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <variant>

#include "audio_context_latency_category.h"
#include "audio_context_render_size_category.h"
#include "audio_sink_options.h"

namespace web_audio {
struct AudioContextOptions {
  std::variant<AudioContextLatencyCategory, double> latencyHint =
      AudioContextLatencyCategory::eInteractive;
  std::optional<float> sampleRate;
  std::variant<std::monostate, std::string, AudioSinkOptions> sinkId;
  std::variant<AudioContextRenderSizeCategory, std::uint32_t> renderSizeHint =
      AudioContextRenderSizeCategory::eDefault;
};
} // namespace web_audio