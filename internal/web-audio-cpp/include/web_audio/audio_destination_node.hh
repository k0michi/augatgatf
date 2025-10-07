#pragma once

#include "audio_node.hh"
#include "detail/common.hh"

#include <stdexcept>

namespace web_audio {
namespace detail {
class AudioGraph;
}

class AudioDestinationNode : public AudioNode {
  WEB_AUDIO_PRIVATE : AudioDestinationNode() = default;

public:
  ~AudioDestinationNode() noexcept = default;

  static std::shared_ptr<AudioDestinationNode>
  create(std::shared_ptr<BaseAudioContext> context,
         std::uint32_t numberOfChannels);

public:
  std::uint32_t getMaxChannelCount() const;

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  friend class detail::AudioGraph;
};
} // namespace web_audio