#pragma once

#include <cstdint>

namespace web_audio {
struct AudioBufferOptions {
  std::uint32_t numberOfChannels = 1;
  std::uint32_t length;
  float sampleRate;
};
} // namespace web_audio