#include "web_audio/convolver_node.hh"

#include <cmath>

#include "web_audio/audio_buffer.hh"
#include "web_audio/base_audio_context.hh"
#include "web_audio/detail/wave_processing.hh"

namespace web_audio {
std::shared_ptr<ConvolverNode>
ConvolverNode::create(std::shared_ptr<BaseAudioContext> context,
                      ConvolverOptions options) {
  auto node = std::shared_ptr<ConvolverNode>(new ConvolverNode());
  node->initialize(context);

  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = options.channelCount.value_or(2);
  node->channelCountMode_ =
      options.channelCountMode.value_or(ChannelCountMode::eClampedMax);
  node->channelInterpretation_ =
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers);

  node->setNormalize(!options.disableNormalization);
  node->setBuffer(options.buffer);

  return node;
}

std::shared_ptr<AudioBuffer> ConvolverNode::getBuffer() const {
  return buffer_;
}

void ConvolverNode::setBuffer(const std::shared_ptr<AudioBuffer> &buffer) {
  // SPEC: If the buffer number of channels is not 1, 2, 4, or if the
  // sample-rate of the buffer is not the same as the sample-rate of its
  // associated BaseAudioContext, a NotSupportedError MUST be thrown.
  if (buffer) {
    auto numberOfChannels = buffer->getNumberOfChannels();
    auto sampleRate = buffer->getSampleRate();
    auto contextSampleRate = getContext()->getSampleRate();

    if (numberOfChannels != 1 && numberOfChannels != 2 &&
        numberOfChannels != 4) {
      throw DOMException(
          "ConvolverNode: buffer number of channels must be 1, 2, or 4",
          "NotSupportedError");
    }

    if (sampleRate != contextSampleRate) {
      throw DOMException(
          "ConvolverNode: buffer sample rate must match context sample rate",
          "NotSupportedError");
    }
  }

  buffer_ = buffer;
  // SPEC: Acquire the content of the AudioBuffer.
  bufferCopy_ = buffer ? buffer->copy() : nullptr;

  if (normalize_ && bufferCopy_) {
    auto scale = calculateNormalizationScale(bufferCopy_);
    auto numberOfChannels = bufferCopy_->getNumberOfChannels();
    auto length = bufferCopy_->getLength();

    for (std::uint32_t ch = 0; ch < numberOfChannels; ++ch) {
      auto &&channelData = bufferCopy_->getChannelData(ch);
      for (std::uint32_t i = 0; i < length; ++i) {
        channelData[i] *= static_cast<float>(scale);
      }
    }
  }

  if (bufferCopy_) {
    convolvers_.resize(bufferCopy_->getNumberOfChannels());

    for (std::uint32_t ch = 0; ch < bufferCopy_->getNumberOfChannels(); ++ch) {
      convolvers_[ch] = std::make_unique<detail::Convolver<float>>(
          bufferCopy_->getChannelData(ch),
          getContext()->getRenderQuantumSize());
    }

    // Copy convolver, so that the states are independent
    if (bufferCopy_->getNumberOfChannels() == 1) {
      convolvers_.push_back(std::make_unique<detail::Convolver<float>>(
          bufferCopy_->getChannelData(0),
          getContext()->getRenderQuantumSize()));
    }
  } else {
    convolvers_.clear();
  }
}

bool ConvolverNode::getNormalize() const { return normalize_; }

void ConvolverNode::setNormalize(bool normalize) { normalize_ = normalize; }

void ConvolverNode::process(const std::vector<detail::RenderQuantum> &inputs,
                            std::vector<detail::RenderQuantum> &outputs,
                            const detail::ParamCollection &params) {
  auto &input = inputs[0];
  auto &output = outputs[0];

  if (bufferCopy_->getNumberOfChannels() == 1) {
    if (input.getNumberOfChannels() == 1) {
      output = detail::RenderQuantum(1, input.getLength());
      convolvers_[0]->process(input[0], output[0]);
    } else { // input.getNumberOfChannels == 2
      output = detail::RenderQuantum(2, input.getLength());
      convolvers_[0]->process(input[0], output[0]);
      convolvers_[1]->process(input[1], output[1]);
    }
  } else if (bufferCopy_->getNumberOfChannels() == 2) {
    if (input.getNumberOfChannels() == 1) {
      output = detail::RenderQuantum(2, input.getLength());
      convolvers_[0]->process(input[0], output[0]);
      convolvers_[1]->process(input[0], output[1]);
    } else { // input.getNumberOfChannels == 2
      output = detail::RenderQuantum(2, input.getLength());
      convolvers_[0]->process(input[0], output[0]);
      convolvers_[1]->process(input[1], output[1]);
    }
  } else { // bufferCopy_->getNumberOfChannels() == 4
    if (input.getNumberOfChannels() == 1) {
      output = detail::RenderQuantum(2, input.getLength());
      std::vector<float> temp;
      convolvers_[0]->process(input[0], output[0]);
      convolvers_[2]->process(input[0], temp);

      for (std::size_t i = 0; i < temp.size(); ++i) {
        output[0][i] += temp[i];
        output[0][i] *= 0.5f;
      }

      convolvers_[1]->process(input[0], output[1]);
      convolvers_[3]->process(input[0], temp);

      for (std::size_t i = 0; i < temp.size(); ++i) {
        output[1][i] += temp[i];
        output[1][i] *= 0.5f;
      }
    } else { // input.getNumberOfChannels == 2
      output = detail::RenderQuantum(2, input.getLength());
      std::vector<float> temp;
      convolvers_[0]->process(input[0], output[0]);
      convolvers_[2]->process(input[1], temp);

      for (std::size_t i = 0; i < temp.size(); ++i) {
        output[0][i] += temp[i];
        output[0][i] *= 0.5f;
      }

      convolvers_[1]->process(input[0], output[1]);
      convolvers_[3]->process(input[1], temp);

      for (std::size_t i = 0; i < temp.size(); ++i) {
        output[1][i] += temp[i];
        output[1][i] *= 0.5f;
      }
    }
  }
}

double ConvolverNode::calculateNormalizationScale(
    std::shared_ptr<AudioBuffer> buffer) {
  constexpr auto GainCalibration = 0.00125;
  constexpr auto GainCalibrationSampleRate = 44100;
  constexpr auto MinPower = 0.000125;

  const auto numberOfChannels = buffer->getNumberOfChannels();
  const auto length = buffer->getLength();

  auto power = 0.0;

  for (std::int32_t i = 0; i < numberOfChannels; ++i) {
    auto channelPower = 0;
    auto &&channelData = buffer->getChannelData(i);

    for (std::int32_t j = 0; j < length; ++j) {
      const auto sample = channelData[j];
      channelPower += sample * sample;
    }

    power += channelPower;
  }

  power = std::sqrt(power / (numberOfChannels * length));

  if (!std::isfinite(power) || std::isnan(power) || power < MinPower) {
    power = MinPower;
  }

  auto scale = 1 / power;

  scale *= GainCalibration;

  if (buffer->getSampleRate()) {
    scale *= GainCalibrationSampleRate / buffer->getSampleRate();
  }

  if (numberOfChannels == 4) {
    scale *= 0.5;
  }

  return scale;
}
} // namespace web_audio
