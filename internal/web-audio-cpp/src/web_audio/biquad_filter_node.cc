#include "web_audio/biquad_filter_node.hh"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

#include "web_audio/audio_param.hh"
#include "web_audio/base_audio_context.hh"
#include "web_audio/detail/vector_helper.hh"

namespace web_audio {
std::shared_ptr<BiquadFilterNode>
BiquadFilterNode::create(std::shared_ptr<BaseAudioContext> context,
                         BiquadFilterOptions options) {
  auto node = std::shared_ptr<BiquadFilterNode>(new BiquadFilterNode());
  node->initialize(context);

  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = options.channelCount.value_or(2);
  node->channelCountMode_ =
      options.channelCountMode.value_or(ChannelCountMode::eMax);
  node->channelInterpretation_ =
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers);

  node->Q_ = AudioParam::create(node, 1, std::numeric_limits<float>::lowest(),
                                std::numeric_limits<float>::max(),
                                AutomationRate::eARate, true);
  node->Q_->setValue(options.Q);

  node->detune_ = AudioParam::create(node, 0, -153600, 153600,
                                     AutomationRate::eARate, true);
  node->detune_->setValue(options.detune);

  auto nyquist = context->getSampleRate() / 2;
  node->frequency_ =
      AudioParam::create(node, 350, 0, nyquist, AutomationRate::eARate, true);
  node->frequency_->setValue(options.frequency);

  node->gain_ =
      AudioParam::create(node, 0, std::numeric_limits<float>::lowest(), 1541,
                         AutomationRate::eARate, true);
  node->gain_->setValue(options.gain);

  node->type_ = options.type;

  node->computeCoefficients(node->type_, context->getSampleRate(),
                            options.frequency, options.gain, options.Q,
                            node->a_, node->b_);
  return node;
}

BiquadFilterType BiquadFilterNode::getType() const { return type_; }

void BiquadFilterNode::setType(BiquadFilterType type) {
  if (type < BiquadFilterType::eLowpass || type > BiquadFilterType::eAllpass) {
    // Chrome and Firefox ignore invalid values
    return;
  }

  type_ = type;
}

std::shared_ptr<AudioParam> BiquadFilterNode::getFrequency() const {
  return frequency_;
}

std::shared_ptr<AudioParam> BiquadFilterNode::getDetune() const {
  return detune_;
}

std::shared_ptr<AudioParam> BiquadFilterNode::getQ() const { return Q_; }

std::shared_ptr<AudioParam> BiquadFilterNode::getGain() const { return gain_; }

void BiquadFilterNode::getFrequencyResponse(
    const std::vector<float> &frequencyHz, std::vector<float> &magResponse,
    std::vector<float> &phaseResponse) {
  for (std::size_t i = 0; i < frequencyHz.size(); ++i) {
    auto f = frequencyHz[i];
    auto omega = 2 * std::numbers::pi * f / getContext()->getSampleRate();
    auto z = std::complex<float>(std::cos(omega), std::sin(omega));

    std::complex<float> H;
    transferFrequency(a_, b_, z, H);

    magResponse[i] = std::abs(H);
    phaseResponse[i] = std::arg(H);
  }
}

void BiquadFilterNode::process(const std::vector<detail::RenderQuantum> &inputs,
                               std::vector<detail::RenderQuantum> &outputs,
                               const detail::ParamCollection &params) {
  auto &input = inputs[0];
  auto &output = outputs[0];

  for (std::uint32_t i = 0; i < output.getLength(); ++i) {
    auto f = params.getValue(frequency_, i);
    auto d = params.getValue(detune_, i);
    auto q = params.getValue(Q_, i);
    auto g = params.getValue(gain_, i);
    auto computedFrequency = static_cast<float>(f * std::pow(2, d / 1200));
    auto nyquist = getContext()->getSampleRate() / 2;
    computedFrequency = std::clamp(computedFrequency, 0.0f, nyquist);

    auto F_s = getContext()->getSampleRate();
    auto f_0 = computedFrequency;
    auto G = g;
    auto Q = q;

    if (type_ == BiquadFilterType::eLowshelf ||
        type_ == BiquadFilterType::eHighshelf) {
      // NOTE: is qMax safe?
      float qMax = std::log10(std::numeric_limits<float>::max()) * 20;
      Q = std::clamp(Q, -qMax, qMax);
    }

    if (type_ == BiquadFilterType::eBandpass ||
        type_ == BiquadFilterType::eNotch ||
        type_ == BiquadFilterType::eAllpass ||
        type_ == BiquadFilterType::ePeaking) {
      Q = std::clamp(Q, 0.f, std::numeric_limits<float>::max());
    }

    computeCoefficients(type_, F_s, f_0, G, Q, a_, b_);

    for (std::uint32_t ch = 0; ch < output.getNumberOfChannels(); ++ch) {
      auto prevX = detail::VectorHelper::getOrDefault(
          x_, ch, std::make_tuple(0.0f, 0.0f, 0.0f));
      auto prevY = detail::VectorHelper::getOrDefault(
          y_, ch, std::make_tuple(0.0f, 0.0f, 0.0f));
      auto x =
          std::make_tuple(input[ch][i], std::get<0>(prevX), std::get<1>(prevX));
      auto y = std::make_tuple(0.0f, std::get<0>(prevY), std::get<1>(prevY));
      transfer(a_, b_, x, y);
      output[ch][i] = std::get<0>(y);
      web_audio::detail::VectorHelper::resizeAndSet(x_, ch, x);
      web_audio::detail::VectorHelper::resizeAndSet(y_, ch, y);
    }
  }
}

std::vector<std::shared_ptr<AudioParam>> BiquadFilterNode::getParams() const {
  return {frequency_, detune_, Q_, gain_};
}

void BiquadFilterNode::transfer(const std::tuple<float, float, float> &a,
                                const std::tuple<float, float, float> &b,
                                const std::tuple<float, float, float> &x,
                                std::tuple<float, float, float> &y) {
  const auto &a_0 = std::get<0>(a);
  const auto &a_1 = std::get<1>(a);
  const auto &a_2 = std::get<2>(a);
  const auto &b_0 = std::get<0>(b);
  const auto &b_1 = std::get<1>(b);
  const auto &b_2 = std::get<2>(b);
  const auto &x_0 = std::get<0>(x);
  const auto &x_1 = std::get<1>(x);
  const auto &x_2 = std::get<2>(x);
  const auto &y_1 = std::get<1>(y);
  const auto &y_2 = std::get<2>(y);
  const auto y_0 =
      (b_0 * x_0 + b_1 * x_1 + b_2 * x_2 - a_1 * y_1 - a_2 * y_2) / a_0;
  y = std::make_tuple(y_0, y_1, y_2);
}

void BiquadFilterNode::transferFrequency(
    const std::tuple<float, float, float> &a,
    const std::tuple<float, float, float> &b, const std::complex<float> &z,
    std::complex<float> &H) {
  H = (std::get<0>(b) + std::get<1>(b) / z + std::get<2>(b) / (z * z)) /
      (std::get<0>(a) + std::get<1>(a) / z + std::get<2>(a) / (z * z));
}

void BiquadFilterNode::computeIntermediate(float F_s, float f_0, float G,
                                           float Q, float &A, float &omega_0,
                                           float &alpha_Q, float &alpha_Q_dB,
                                           float &S, float &alpha_S) {
  A = std::pow(10, G / 40);
  omega_0 = 2 * std::numbers::pi * f_0 / F_s;
  alpha_Q = std::sin(omega_0) / (2 * Q);
  alpha_Q_dB = std::sin(omega_0) / (2 * std::pow(10, Q / 20));
  S = 1;
  alpha_S = std::sin(omega_0) / 2 * std::sqrt((A + 1 / A) * (1 / S - 1) + 2);
}

void BiquadFilterNode::computeCoefficients(BiquadFilterType type, float F_s,
                                           float f_0, float G, float Q,
                                           std::tuple<float, float, float> &a,
                                           std::tuple<float, float, float> &b) {
  switch (type) {
  case BiquadFilterType::eLowpass:
    lowpass(F_s, f_0, G, Q, a, b);
    break;
  case BiquadFilterType::eHighpass:
    highpass(F_s, f_0, G, Q, a, b);
    break;
  case BiquadFilterType::eBandpass:
    bandpass(F_s, f_0, G, Q, a, b);
    break;
  case BiquadFilterType::eNotch:
    notch(F_s, f_0, G, Q, a, b);
    break;
  case BiquadFilterType::eAllpass:
    allpass(F_s, f_0, G, Q, a, b);
    break;
  case BiquadFilterType::ePeaking:
    peaking(F_s, f_0, G, Q, a, b);
    break;
  case BiquadFilterType::eLowshelf:
    lowshelf(F_s, f_0, G, Q, a, b);
    break;
  case BiquadFilterType::eHighshelf:
    highshelf(F_s, f_0, G, Q, a, b);
    break;
  default:
    std::abort();
  }
}

void BiquadFilterNode::lowpass(float F_s, float f_0, float G, float Q,
                               std::tuple<float, float, float> &a,
                               std::tuple<float, float, float> &b) {
  float A, omega_0, alpha_Q, alpha_Q_dB, S, alpha_S;
  computeIntermediate(F_s, f_0, G, Q, A, omega_0, alpha_Q, alpha_Q_dB, S,
                      alpha_S);

  auto b_0 = (1 - std::cos(omega_0)) / 2;
  auto b_1 = 1 - std::cos(omega_0);
  auto b_2 = (1 - std::cos(omega_0)) / 2;
  auto a_0 = 1 + alpha_Q_dB;
  auto a_1 = -2 * std::cos(omega_0);
  auto a_2 = 1 - alpha_Q_dB;
  a = std::make_tuple(a_0, a_1, a_2);
  b = std::make_tuple(b_0, b_1, b_2);
}

void BiquadFilterNode::highpass(float F_s, float f_0, float G, float Q,
                                std::tuple<float, float, float> &a,
                                std::tuple<float, float, float> &b) {
  float A, omega_0, alpha_Q, alpha_Q_dB, S, alpha_S;
  computeIntermediate(F_s, f_0, G, Q, A, omega_0, alpha_Q, alpha_Q_dB, S,
                      alpha_S);

  auto b_0 = (1 + std::cos(omega_0)) / 2;
  auto b_1 = -(1 + std::cos(omega_0));
  auto b_2 = (1 + std::cos(omega_0)) / 2;
  auto a_0 = 1 + alpha_Q_dB;
  auto a_1 = -2 * std::cos(omega_0);
  auto a_2 = 1 - alpha_Q_dB;
  a = std::make_tuple(a_0, a_1, a_2);
  b = std::make_tuple(b_0, b_1, b_2);
}

void BiquadFilterNode::bandpass(float F_s, float f_0, float G, float Q,
                                std::tuple<float, float, float> &a,
                                std::tuple<float, float, float> &b) {
  float A, omega_0, alpha_Q, alpha_Q_dB, S, alpha_S;
  computeIntermediate(F_s, f_0, G, Q, A, omega_0, alpha_Q, alpha_Q_dB, S,
                      alpha_S);

  auto b_0 = alpha_Q;
  auto b_1 = 0;
  auto b_2 = -alpha_Q;
  auto a_0 = 1 + alpha_Q;
  auto a_1 = -2 * std::cos(omega_0);
  auto a_2 = 1 - alpha_Q;
  a = std::make_tuple(a_0, a_1, a_2);
  b = std::make_tuple(b_0, b_1, b_2);
}

void BiquadFilterNode::notch(float F_s, float f_0, float G, float Q,
                             std::tuple<float, float, float> &a,
                             std::tuple<float, float, float> &b) {
  float A, omega_0, alpha_Q, alpha_Q_dB, S, alpha_S;
  computeIntermediate(F_s, f_0, G, Q, A, omega_0, alpha_Q, alpha_Q_dB, S,
                      alpha_S);

  auto b_0 = 1;
  auto b_1 = -2 * std::cos(omega_0);
  auto b_2 = 1;
  auto a_0 = 1 + alpha_Q;
  auto a_1 = -2 * std::cos(omega_0);
  auto a_2 = 1 - alpha_Q;
  a = std::make_tuple(a_0, a_1, a_2);
  b = std::make_tuple(b_0, b_1, b_2);
}

void BiquadFilterNode::allpass(float F_s, float f_0, float G, float Q,
                               std::tuple<float, float, float> &a,
                               std::tuple<float, float, float> &b) {
  float A, omega_0, alpha_Q, alpha_Q_dB, S, alpha_S;
  computeIntermediate(F_s, f_0, G, Q, A, omega_0, alpha_Q, alpha_Q_dB, S,
                      alpha_S);

  auto b_0 = 1 - alpha_Q;
  auto b_1 = -2 * std::cos(omega_0);
  auto b_2 = 1 + alpha_Q;
  auto a_0 = 1 + alpha_Q;
  auto a_1 = -2 * std::cos(omega_0);
  auto a_2 = 1 - alpha_Q;
  a = std::make_tuple(a_0, a_1, a_2);
  b = std::make_tuple(b_0, b_1, b_2);
}

void BiquadFilterNode::peaking(float F_s, float f_0, float G, float Q,
                               std::tuple<float, float, float> &a,
                               std::tuple<float, float, float> &b) {
  float A, omega_0, alpha_Q, alpha_Q_dB, S, alpha_S;
  computeIntermediate(F_s, f_0, G, Q, A, omega_0, alpha_Q, alpha_Q_dB, S,
                      alpha_S);

  auto b_0 = 1 + alpha_Q_dB * A;
  auto b_1 = -2 * std::cos(omega_0);
  auto b_2 = 1 - alpha_Q * A;
  auto a_0 = 1 + alpha_Q / A;
  auto a_1 = -2 * std::cos(omega_0);
  auto a_2 = 1 - alpha_Q / A;
  a = std::make_tuple(a_0, a_1, a_2);
  b = std::make_tuple(b_0, b_1, b_2);
}

void BiquadFilterNode::lowshelf(float F_s, float f_0, float G, float S,
                                std::tuple<float, float, float> &a,
                                std::tuple<float, float, float> &b) {
  float A, omega_0, alpha_Q, alpha_Q_dB, S_unused, alpha_S;
  computeIntermediate(F_s, f_0, G, 1, A, omega_0, alpha_Q, alpha_Q_dB, S_unused,
                      alpha_S);

  auto b_0 =
      A * ((A + 1) - (A - 1) * std::cos(omega_0) + 2 * alpha_S * std::sqrt(A));
  auto b_1 = 2 * A * ((A - 1) - (A + 1) * std::cos(omega_0));
  auto b_2 =
      A * ((A + 1) - (A - 1) * std::cos(omega_0) - 2 * alpha_S * std::sqrt(A));
  auto a_0 = (A + 1) + (A - 1) * std::cos(omega_0) + 2 * alpha_S * std::sqrt(A);
  auto a_1 = -2 * ((A - 1) + (A + 1) * std::cos(omega_0));
  auto a_2 = (A + 1) + (A - 1) * std::cos(omega_0) - 2 * alpha_S * std::sqrt(A);
  a = std::make_tuple(a_0, a_1, a_2);
  b = std::make_tuple(b_0, b_1, b_2);
}

void BiquadFilterNode::highshelf(float F_s, float f_0, float G, float S,
                                 std::tuple<float, float, float> &a,
                                 std::tuple<float, float, float> &b) {
  float A, omega_0, alpha_Q, alpha_Q_dB, S_unused, alpha_S;
  computeIntermediate(F_s, f_0, G, 1, A, omega_0, alpha_Q, alpha_Q_dB, S_unused,
                      alpha_S);

  auto b_0 =
      A * ((A + 1) + (A - 1) * std::cos(omega_0) + 2 * alpha_S * std::sqrt(A));
  auto b_1 = -2 * A * ((A - 1) + (A + 1) * std::cos(omega_0));
  auto b_2 =
      A * ((A + 1) + (A - 1) * std::cos(omega_0) - 2 * alpha_S * std::sqrt(A));
  auto a_0 = (A + 1) - (A - 1) * std::cos(omega_0) + 2 * alpha_S * std::sqrt(A);
  auto a_1 = 2 * ((A - 1) - (A + 1) * std::cos(omega_0));
  auto a_2 = (A + 1) - (A - 1) * std::cos(omega_0) - 2 * alpha_S * std::sqrt(A);
  a = std::make_tuple(a_0, a_1, a_2);
  b = std::make_tuple(b_0, b_1, b_2);
}
} // namespace web_audio