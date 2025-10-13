#include "web_audio/constant_source_node.hh"

#include <limits>

#include "web_audio/audio_param.hh"

namespace web_audio {
std::shared_ptr<ConstantSourceNode>
ConstantSourceNode::create(std::shared_ptr<BaseAudioContext> context,
                           const ConstantSourceOptions &options) {
  auto node = std::shared_ptr<ConstantSourceNode>(new ConstantSourceNode());
  node->initialize(context);

  node->numberOfInputs_ = 0;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = 2;
  node->channelCountMode_ = ChannelCountMode::eMax;
  node->channelInterpretation_ = ChannelInterpretation::eSpeakers;

  node->offset_ = AudioParam::create(
      node, 1, std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::max(), AutomationRate::eARate, true);
  node->offset_->setValue(options.offset);

  return node;
}

std::shared_ptr<AudioParam> ConstantSourceNode::getOffset() const {
  return offset_;
}

std::vector<std::shared_ptr<AudioParam>> ConstantSourceNode::getParams() const {
  return {offset_};
}

void ConstantSourceNode::process(
    const std::vector<detail::RenderQuantum> &inputs,
    std::vector<detail::RenderQuantum> &outputs,
    const detail::ParamCollection &params) {
  auto &output = outputs[0];
  output = detail::RenderQuantum(1, output.getLength());

  for (std::uint32_t i = 0; i < output.getLength(); ++i) {
    float value = params.getValue(offset_, i);
    output[0][i] = value;
  }
}
} // namespace web_audio
