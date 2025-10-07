#include "web_audio/audio_destination_node.hh"

namespace web_audio {
std::shared_ptr<AudioDestinationNode>
AudioDestinationNode::create(std::shared_ptr<BaseAudioContext> context,
                             std::uint32_t numberOfChannels) {
  auto node = std::shared_ptr<AudioDestinationNode>(new AudioDestinationNode());
  node->initialize(context);
  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = numberOfChannels;
  node->channelCountMode_ = ChannelCountMode::eExplicit;
  node->channelInterpretation_ = ChannelInterpretation::eSpeakers;
  return node;
}

std::uint32_t AudioDestinationNode::getMaxChannelCount() const {
  // TODO
  return 2;
}

void AudioDestinationNode::process(
    const std::vector<detail::RenderQuantum> &inputs,
    std::vector<detail::RenderQuantum> &outputs,
    const detail::ParamCollection &params) {
  auto &output = outputs[0];

  for (auto &input : inputs) {
    output.add(input, channelInterpretation_);
  }
}
} // namespace web_audio