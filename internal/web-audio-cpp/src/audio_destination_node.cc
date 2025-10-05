#include "web_audio/audio_destination_node.h"

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