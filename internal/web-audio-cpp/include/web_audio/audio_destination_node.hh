#pragma once

#include "audio_node.hh"
#include "detail/common.hh"

#include <stdexcept>

namespace web_audio {
class AudioDestinationNode : public AudioNode {
  WEB_AUDIO_PRIVATE : AudioDestinationNode() = default;

public:
  ~AudioDestinationNode() noexcept = default;

public:
  std::uint32_t getMaxChannelCount() const;

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  friend class AudioContext;
  friend class OfflineAudioContext;
  friend class BaseAudioContext;
};
} // namespace web_audio