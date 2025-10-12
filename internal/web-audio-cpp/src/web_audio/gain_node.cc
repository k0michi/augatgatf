#include "web_audio/gain_node.hh"

#include "web_audio/audio_param.hh"

#include <limits>

namespace web_audio {
std::shared_ptr<GainNode>
GainNode::create(std::shared_ptr<BaseAudioContext> context,
                 GainOptions options) {
  auto node = std::shared_ptr<GainNode>(new GainNode());
  node->initialize(context);

  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = options.channelCount.value_or(2);
  node->channelCountMode_ =
      options.channelCountMode.value_or(ChannelCountMode::eMax);
  node->channelInterpretation_ =
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers);

  node->gain_ = AudioParam::create(
      node, 1, std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::max(), AutomationRate::eARate, true);
  node->gain_->setValue(options.gain);

  return node;
}

std::shared_ptr<AudioParam> GainNode::getGain() const { return gain_; }

void GainNode::process(const std::vector<detail::RenderQuantum> &inputs,
                       std::vector<detail::RenderQuantum> &outputs,
                       const detail::ParamCollection &params) {
  auto &input = inputs[0];
  auto &output = outputs[0];

  for (std::uint32_t i = 0; i < output.getLength(); ++i) {
    auto gain = params.getValue(gain_, i);

    for (std::uint32_t ch = 0; ch < output.getNumberOfChannels(); ++ch) {
      output[ch][i] = input[ch][i] * gain;
    }
  }
}

std::vector<std::shared_ptr<AudioParam>> GainNode::getParams() const {
  return {gain_};
}
} // namespace web_audio