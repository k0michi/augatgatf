#include "web_audio/stereo_panner_node.hh"

#include <algorithm>
#include <cmath>
#include <numbers>

#include "web_audio/audio_param.hh"

namespace web_audio {
std::shared_ptr<StereoPannerNode>
StereoPannerNode::create(std::shared_ptr<BaseAudioContext> context,
                         const StereoPannerOptions &options) {
  auto node = std::shared_ptr<StereoPannerNode>(new StereoPannerNode());
  node->initialize(context);

  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = options.channelCount.value_or(2);
  node->channelCountMode_ =
      options.channelCountMode.value_or(ChannelCountMode::eClampedMax);
  node->channelInterpretation_ =
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers);

  node->pan_ = AudioParam::create(node, 0, -1, 1, AutomationRate::eARate, true);
  node->pan_->setValue(options.pan);

  return node;
}

std::shared_ptr<AudioParam> StereoPannerNode::getPan() const { return pan_; }

std::vector<std::shared_ptr<AudioParam>> StereoPannerNode::getParams() const {
  return {pan_};
}

void StereoPannerNode::process(const std::vector<detail::RenderQuantum> &inputs,
                               std::vector<detail::RenderQuantum> &outputs,
                               const detail::ParamCollection &params) {
  auto &input = inputs[0];
  auto &output = outputs[0];

  for (std::uint32_t i = 0; i < input.getLength(); i++) {
    auto pan = params.getValue(pan_, i);
    pan = std::max(-1.0f, pan);
    pan = std::min(1.0f, pan);
    float x;

    if (input.getNumberOfChannels() == 1) {
      x = (pan + 1) / 2;
    } else {
      if (pan <= 0) {
        x = pan + 1;
      } else {
        x = pan;
      }
    }

    auto gainL = std::cos(x * std::numbers::pi / 2);
    auto gainR = std::sin(x * std::numbers::pi / 2);

    if (input.getNumberOfChannels() == 1) {
      output[0][i] = input[0][i] * gainL;
      output[1][i] = input[0][i] * gainR;
    } else {
      if (pan <= 0) {
        output[0][i] = input[0][i] + input[1][i] * gainL;
        output[1][i] = input[1][i] * gainR;
      } else {
        output[0][i] = input[0][i] * gainL;
        output[1][i] = input[1][i] + input[0][i] * gainR;
      }
    }
  }
}
} // namespace web_audio
