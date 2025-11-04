#pragma once

#include <memory>

#include "audio_node_options.hh"

namespace web_audio {
class AudioBuffer;

struct AudioBufferSourceOptions : public AudioNodeOptions {
  std::shared_ptr<AudioBuffer> buffer = nullptr;
  float detune = 0;
  bool loop = false;
  double loopEnd = 0;
  double loopStart = 0;
  float playbackRate = 1;
};
} // namespace web_audio