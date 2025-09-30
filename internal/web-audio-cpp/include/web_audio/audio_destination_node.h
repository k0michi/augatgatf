#pragma once

#include "audio_node.h"

namespace web_audio {
class AudioDestinationNode : public AudioNode {
private:
  AudioDestinationNode() = default;

public:
  ~AudioDestinationNode() noexcept = default;

public:
  std::uint32_t getMaxChannelCount() const;

  friend class AudioContext;
  friend class OfflineAudioContext;
  friend class BaseAudioContext;
};
} // namespace web_audio

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio {
std::uint32_t AudioDestinationNode::getMaxChannelCount() const {
  // TODO
  return 2;
}
} // namespace web_audio
#endif