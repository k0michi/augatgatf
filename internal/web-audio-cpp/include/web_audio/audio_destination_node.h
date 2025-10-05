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