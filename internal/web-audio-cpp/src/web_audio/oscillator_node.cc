#include "web_audio/oscillator_node.hh"

#include "web_audio/audio_param.hh"
#include "web_audio/base_audio_context.hh"
#include "web_audio/periodic_wave.hh"

#include <numbers>

namespace web_audio {
std::shared_ptr<OscillatorNode>
OscillatorNode::create(std::shared_ptr<BaseAudioContext> context,
                       const OscillatorOptions &options) {
  auto node = std::shared_ptr<OscillatorNode>(new OscillatorNode());
  node->initialize(context);
  node->numberOfInputs_ = 0;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = options.channelCount.value_or(2);
  node->channelCountMode_ =
      options.channelCountMode.value_or(ChannelCountMode::eMax);
  node->channelInterpretation_ =
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers);

  auto nyquist = context->getSampleRate() / 2;

  if (options.periodicWave) {
    node->type_ = OscillatorType::eCustom;
  } else {
    node->type_ = options.type;
  }

  node->frequency_ = AudioParam::create(node, 440, -nyquist, nyquist,
                                        AutomationRate::eARate, true);
  node->frequency_->setValue(options.frequency);
  node->detune_ = AudioParam::create(node, 0, -153600, 153600,
                                     AutomationRate::eARate, true);
  node->detune_->setValue(options.detune);

  // SPEC: The type of oscillator to be constructed. If this is set to "custom"
  // without also specifying a periodicWave, then an InvalidStateError exception
  // MUST be thrown.
  if (options.type == OscillatorType::eCustom && !options.periodicWave) {
    throw DOMException(
        "OscillatorNode: periodicWave must be specified when type is 'custom'",
        "InvalidStateError");
  }

  node->periodicWave_ = options.periodicWave;

  return node;
}

OscillatorType OscillatorNode::getType() const { return type_; }

void OscillatorNode::setType(OscillatorType type) {
  // SPEC:  It may directly be set to any of the type constant values except for
  // "custom". Doing so MUST throw an InvalidStateError exception.
  if (type == OscillatorType::eCustom) {
    throw DOMException("OscillatorNode: cannot set type to 'custom' directly",
                       "InvalidStateError");
  }

  type_ = type;
}

std::shared_ptr<AudioParam> OscillatorNode::getFrequency() const {
  return frequency_;
}

std::shared_ptr<AudioParam> OscillatorNode::getDetune() const {
  return detune_;
}

void OscillatorNode::setPeriodicWave(
    std::shared_ptr<PeriodicWave> periodicWave) {
  // SPEC: The setPeriodicWave() method can be used to set a custom waveform,
  // which results in this attribute being set to "custom".

  periodicWave_ = periodicWave;
  type_ = OscillatorType::eCustom;
}

void OscillatorNode::process(const std::vector<detail::RenderQuantum> &inputs,
                             std::vector<detail::RenderQuantum> &outputs,
                             const detail::ParamCollection &params) {
  auto &output = outputs[0];
  output = detail::RenderQuantum(1, output.getLength());

  for (std::uint32_t i = 0; i < output.getLength(); ++i) {
    auto frequency = params.getValue(frequency_, i);
    auto detune = params.getValue(detune_, i);
    auto computedOscFrequency = frequency * std::pow(2.0f, detune / 1200.0f);
    auto phaseIncrement = computedOscFrequency / getContext()->getSampleRate();
    phase_ += phaseIncrement;
    if (phase_ >= 1.0f) {
      phase_ -= 1.0f;
    }

    if (type_ == OscillatorType::eSine) {
      output[0][i] = std::sin(phase_ * 2.0f * std::numbers::pi);
    } else if (type_ == OscillatorType::eSquare) {
      output[0][i] = phase_ < 0.5f ? 1.0f : -1.0f;
    } else if (type_ == OscillatorType::eSawtooth) {
      output[0][i] = 1.0f - 2.0f * phase_;
    } else if (type_ == OscillatorType::eTriangle) {
      output[0][i] =
          phase_ < 0.5f ? 2.0f * phase_ : 1.0f - 2.0f * (phase_ - 0.5f);
    } else if (type_ == OscillatorType::eCustom && periodicWave_) {
      output[0][i] = periodicWave_->getSample(phase_);
    } else {
      output[0][i] = 0.0f;
    }
  }
}

std::vector<std::shared_ptr<AudioParam>> OscillatorNode::getParams() const {
  return {frequency_, detune_};
}
} // namespace web_audio