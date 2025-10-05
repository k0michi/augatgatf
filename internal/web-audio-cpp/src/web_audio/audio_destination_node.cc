#include "web_audio/audio_destination_node.hh"

namespace web_audio {
std::uint32_t AudioDestinationNode::getMaxChannelCount() const {
  // TODO
  return 2;
}

void AudioDestinationNode::process(
    const std::vector<detail::RenderQuantum> &inputs,
    std::vector<detail::RenderQuantum> &outputs,
    const detail::ParamCollection &params) {
  throw std::runtime_error("Not implemented");
}
} // namespace web_audio