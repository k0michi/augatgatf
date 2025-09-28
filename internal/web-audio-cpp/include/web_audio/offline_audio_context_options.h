#pragma once

#include <cstdint>
#include <variant>

#include "audio_context_render_size_category.h"

namespace web_audio {
struct OfflineAudioContextOptions {
  std::uint32_t numberOfChannels = 1;
  std::uint32_t length;
  float sampleRate;
  std::variant<AudioContextRenderSizeCategory, std::uint32_t> renderSizeHint =
      AudioContextRenderSizeCategory::eDefault;
};
} // namespace web_audio