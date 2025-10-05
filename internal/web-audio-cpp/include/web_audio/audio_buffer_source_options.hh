#pragma once

#include <memory>

namespace web_audio {
class AudioBuffer;

struct AudioBufferSourceOptions {
  std::shared_ptr<AudioBuffer> buffer = nullptr;
  float detune = 0;
  bool loop = false;
  double loopEnd = 0;
  double loopStart = 0;
  float playbackRate = 1;
};
} // namespace web_audio