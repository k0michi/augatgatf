#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <variant>

#include "audio_context_latency_category.hh"
#include "audio_context_render_size_category.hh"
#include "audio_sink_info.hh"

namespace web_audio {
struct AudioContextOptions {
  std::variant<AudioContextLatencyCategory, double> latencyHint =
      AudioContextLatencyCategory::eInteractive;
  std::optional<float> sampleRate;
  std::variant<std::monostate, std::string, AudioSinkInfo> sinkId;
  std::variant<AudioContextRenderSizeCategory, std::uint32_t> renderSizeHint =
      AudioContextRenderSizeCategory::eDefault;
};
} // namespace web_audio