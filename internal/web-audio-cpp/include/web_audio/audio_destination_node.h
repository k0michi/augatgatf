#pragma once

#include "audio_node.h"

#include <stdexcept>

namespace web_audio {
class AudioDestinationNode : public AudioNode {
private:
  AudioDestinationNode() = default;

public:
  ~AudioDestinationNode() noexcept = default;

public:
  std::uint32_t getMaxChannelCount() const;

  void process(const std::vector<details::RenderQuantum> &inputs,
               std::vector<details::RenderQuantum> &outputs,
               const details::ParamCollection &params) override;

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

void AudioDestinationNode::process(
    const std::vector<details::RenderQuantum> &inputs,
    std::vector<details::RenderQuantum> &outputs,
    const details::ParamCollection &params) {
  throw std::runtime_error("Not implemented");
}
} // namespace web_audio
#endif