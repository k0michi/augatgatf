#include "web_audio/wave_shaper_node.hh"

#include "web_audio/base_audio_context.hh"
#include "web_audio/dom_exception.hh"

namespace web_audio {
std::shared_ptr<WaveShaperNode>
WaveShaperNode::create(std::shared_ptr<BaseAudioContext> context,
                       WaveShaperOptions options) {
  auto node = std::shared_ptr<WaveShaperNode>(new WaveShaperNode());
  node->initialize(context);

  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = options.channelCount.value_or(2);
  node->channelCountMode_ =
      options.channelCountMode.value_or(ChannelCountMode::eMax);
  node->channelInterpretation_ =
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers);

  node->curve_ = options.curve;
  node->oversample_ = options.oversample;
  return node;
}

std::optional<std::vector<float>> WaveShaperNode::getCurve() const {
  return curve_;
}

void WaveShaperNode::setCurve(const std::optional<std::vector<float>> &curve) {
  // SPEC: A InvalidStateError MUST be thrown if this attribute is set with a
  // Float32Array that has a length less than 2.
  if (curve.has_value() && curve->size() < 2) {
    throw DOMException("WaveShaperNode: curve length must be at least 2",
                       "InvalidStateError");
  }

  curve_ = curve;
}

OverSampleType WaveShaperNode::getOversample() const { return oversample_; }

void WaveShaperNode::setOversample(OverSampleType oversample) {
  oversample_ = oversample;

  if (oversample_ == OverSampleType::eNone) {
    upsampler_.reset();
    downsampler_.reset();
  } else if (oversample_ == OverSampleType::e2x) {
    upsampler_ = std::make_unique<detail::Upsampler2x>(
        getContext()->getRenderQuantumSize());
    downsampler_ = std::make_unique<detail::Downsampler2x>(
        getContext()->getRenderQuantumSize());
  } else if (oversample_ == OverSampleType::e4x) {
    upsampler_ = std::make_unique<detail::Upsampler4x>(
        getContext()->getRenderQuantumSize());
    downsampler_ = std::make_unique<detail::Downsampler4x>(
        getContext()->getRenderQuantumSize());
  }
}

void WaveShaperNode::process(const std::vector<detail::RenderQuantum> &inputs,
                             std::vector<detail::RenderQuantum> &outputs,
                             const detail::ParamCollection &params) {
  auto &input = inputs[0];
  auto &output = outputs[0];

  if (curve_) {
    auto curve = *curve_;
    for (std::uint32_t i = 0; i < output.getLength(); ++i) {
      auto N = curve.size();
      auto v = (N - 1) / 2.0 * (i + 1);
      auto k = static_cast<std::uint32_t>(v);
      auto f = v - k;

      for (std::uint32_t ch = 0; ch < output.getNumberOfChannels(); ++ch) {
        if (v < 0) {
          output[ch][i] = curve[0];
        } else if (v >= N - 1) {
          output[ch][i] = curve[N - 1];
        } else {
          output[ch][i] = (1 - f) * curve[k] + f * curve[k + 1];
        }
      }
    }
  } else {
    output = input;
  }
}
} // namespace web_audio
