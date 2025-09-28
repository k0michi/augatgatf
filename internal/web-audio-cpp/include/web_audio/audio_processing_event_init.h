#pragma once

#include <memory>

namespace web_audio {
class AudioBuffer;

// TODO: Inherit from EventInit
struct AudioProcessingEventInit {
  double playbackTime;
  std::shared_ptr<AudioBuffer> inputBuffer;
  std::shared_ptr<AudioBuffer> outputBuffer;
};
} // namespace web_audio