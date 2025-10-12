#include "web_audio/iir_filter_node.hh"

#include <algorithm>
#include <limits>
#include <numbers>

#include "web_audio/base_audio_context.hh"
#include "web_audio/detail/vector_helper.hh"

namespace web_audio {
std::shared_ptr<IIRFilterNode>
IIRFilterNode::create(std::shared_ptr<BaseAudioContext> context,
                      IIRFilterOptions options) {
  // SPEC: If all of the values are zero, an InvalidStateError MUST be thrown.
  if (std::all_of(options.feedforward.begin(), options.feedforward.end(),
                  [](double v) { return v == 0.0; })) {
    throw DOMException("IIRFilterNode: all of the feedforward values are zero",
                       "InvalidStateError");
  }

  // SPEC: A NotSupportedError MUST be thrown if the array length is 0 or
  // greater than 20.

  if (options.feedforward.size() == 0 || options.feedforward.size() > 20) {
    throw DOMException(
        "IIRFilterNode: feedforward array length is out of range",
        "NotSupportedError");
  }

  if (std::all_of(options.feedback.begin(), options.feedback.end(),
                  [](double v) { return v == 0.0; })) {
    throw DOMException("IIRFilterNode: all of the feedback values are zero",
                       "InvalidStateError");
  }

  if (options.feedback.size() == 0 || options.feedback.size() > 20) {
    throw DOMException("IIRFilterNode: feedback array length is out of range",
                       "NotSupportedError");
  }

  auto node = std::shared_ptr<IIRFilterNode>(new IIRFilterNode());
  node->initialize(context);

  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = options.channelCount.value_or(2);
  node->channelCountMode_ =
      options.channelCountMode.value_or(ChannelCountMode::eMax);
  node->channelInterpretation_ =
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers);

  node->feedforward_ = options.feedforward;
  node->feedback_ = options.feedback;

  return node;
}

void IIRFilterNode::getFrequencyResponse(const std::vector<float> &frequencyHz,
                                         std::vector<float> &magResponse,
                                         std::vector<float> &phaseResponse) {
  // SPEC: The three parameters MUST be Float32Arrays of the same length, or an
  // InvalidAccessError MUST be thrown.
  if (frequencyHz.size() != magResponse.size() ||
      frequencyHz.size() != phaseResponse.size()) {
    throw DOMException("The three parameters must be  the same length.",
                       "InvalidAccessError");
  }

  for (std::size_t i = 0; i < frequencyHz.size(); ++i) {
    auto f = frequencyHz[i];

    // SPEC: If a value in the frequencyHz parameter is not within [0,
    // sampleRate/2], where sampleRate is the value of the sampleRate property
    // of the AudioContext, the corresponding value at the same index of the
    // magResponse array MUST be NaN.
    if (f < 0 || f > getContext()->getSampleRate() / 2 || std::isnan(f) ||
        std::isinf(f)) {
      magResponse[i] = phaseResponse[i] =
          std::numeric_limits<float>::quiet_NaN();
      continue;
    }

    auto omega = 2 * std::numbers::pi * f / getContext()->getSampleRate();
    auto z = std::complex<double>(std::cos(omega), std::sin(omega));

    std::complex<double> H;
    transferFrequency(feedback_, feedforward_, z, H);

    magResponse[i] = std::abs(H);
    phaseResponse[i] = std::arg(H);
  }
}

void IIRFilterNode::process(const std::vector<detail::RenderQuantum> &inputs,
                            std::vector<detail::RenderQuantum> &outputs,
                            const detail::ParamCollection &params) {
  auto &input = inputs[0];
  outputs.resize(1);
  auto &output = outputs[0];

  for (std::uint32_t i = 0; i < output.getLength(); ++i) {
    for (std::uint32_t ch = 0; ch < output.getNumberOfChannels(); ++ch) {
      auto prevX = web_audio::detail::VectorHelper::getOrDefault(
          x_, ch, std::vector<float>{});
      auto prevY = web_audio::detail::VectorHelper::getOrDefault(
          y_, ch, std::vector<double>{});
      auto x = std::vector<float>(feedforward_.size(), 0.0f);
      auto y = std::vector<double>(feedback_.size(), 0.0);

      x[0] = input[ch][i];
      for (std::size_t j = 1; j < x.size(); ++j) {
        if (j - 1 < prevX.size()) {
          x[j] = prevX[j - 1];
        }
      }

      for (std::size_t j = 1; j < y.size(); ++j) {
        if (j - 1 < prevY.size()) {
          y[j] = prevY[j - 1];
        }
      }

      transferTime(feedback_, feedforward_, x, y);

      output[ch][i] = static_cast<float>(y[0]);
      web_audio::detail::VectorHelper::resizeAndSet(x_, ch, std::move(x));
      web_audio::detail::VectorHelper::resizeAndSet(y_, ch, std::move(y));
    }
  }
}

void IIRFilterNode::transferTime(const std::vector<double> &a,
                                 const std::vector<double> &b,
                                 const std::vector<float> &x,
                                 std::vector<double> &y) {
  for (std::size_t i = 0; i < x.size(); ++i) {
    y[0] += b[i] * x[i];
  }

  for (std::size_t i = 1; i < a.size(); ++i) {
    y[0] -= a[i] * y[i];
  }

  y[0] /= a[0];
}

void IIRFilterNode::transferFrequency(const std::vector<double> &a,
                                      const std::vector<double> &b,
                                      const std::complex<double> &z,
                                      std::complex<double> &H) {
  H = 0;

  for (std::size_t i = 0; i < b.size(); ++i) {
    H += b[i] * std::pow(z, -static_cast<double>(i));
  }

  for (std::size_t i = 0; i < a.size(); ++i) {
    H /= a[i] * std::pow(z, -static_cast<double>(i));
  }
}
} // namespace web_audio
