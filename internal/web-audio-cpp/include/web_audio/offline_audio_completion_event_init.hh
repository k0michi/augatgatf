#pragma once

#include <memory>

namespace web_audio {
class AudioBuffer;

// TODO: Inherit from EventInit
struct OfflineAudioCompletionEventInit {
  std::shared_ptr<AudioBuffer> renderedBuffer;
};
} // namespace web_audio