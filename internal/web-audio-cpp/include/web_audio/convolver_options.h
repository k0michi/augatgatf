#pragma once

#include <memory>

#include "audio_node_options.h"

namespace web_audio {
class AudioBuffer;

struct ConvolverOptions : public AudioNodeOptions {
  std::shared_ptr<AudioBuffer> buffer;
  bool disableNormalization = false;
};
} // namespace web_audio