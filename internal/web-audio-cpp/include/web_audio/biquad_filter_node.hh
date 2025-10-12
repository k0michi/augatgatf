#pragma once

#include <tuple>

#include "audio_node.hh"
#include "biquad_filter_options.hh"

namespace web_audio {
class BiquadFilterNode : public AudioNode {
public:
  // SPEC: constructor (BaseAudioContext context, optional BiquadFilterOptions
  // options = {});
  static std::shared_ptr<BiquadFilterNode>
  create(std::shared_ptr<BaseAudioContext> context,
         BiquadFilterOptions options = {});

  // SPEC: attribute BiquadFilterType type;
  BiquadFilterType getType() const;

  void setType(BiquadFilterType type);

  // SPEC: readonly attribute AudioParam frequency;
  std::shared_ptr<AudioParam> getFrequency() const;

  // SPEC: readonly attribute AudioParam detune;
  std::shared_ptr<AudioParam> getDetune() const;

  // SPEC: readonly attribute AudioParam Q;
  std::shared_ptr<AudioParam> getQ() const;

  // SPEC: readonly attribute AudioParam gain;
  std::shared_ptr<AudioParam> getGain() const;

  // SPEC: undefined getFrequencyResponse (Float32Array frequencyHz,
  // Float32Array magResponse, Float32Array phaseResponse);
  void getFrequencyResponse(const std::vector<float> &frequencyHz,
                            std::vector<float> &magResponse,
                            std::vector<float> &phaseResponse);

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  WEB_AUDIO_PRIVATE : BiquadFilterNode() = default;

  std::vector<std::shared_ptr<AudioParam>> getParams() const override;

  void transfer(const std::tuple<float, float, float> &a,
                const std::tuple<float, float, float> &b,
                const std::tuple<float, float, float> &x,
                std::tuple<float, float, float> &y);

  static void computeIntermediate(float F_s, float f_0, float G, float Q,
                                  float &A, float &omega_0, float &alpha_Q,
                                  float &alpha_Q_dB, float &S, float &alpha_S);

  static void lowpass(float F_s, float f_0, float G, float Q,
                      std::tuple<float, float, float> &a,
                      std::tuple<float, float, float> &b);

  static void highpass(float F_s, float f_0, float G, float Q,
                       std::tuple<float, float, float> &a,
                       std::tuple<float, float, float> &b);

  static void bandpass(float F_s, float f_0, float G, float Q,
                       std::tuple<float, float, float> &a,
                       std::tuple<float, float, float> &b);

  static void notch(float F_s, float f_0, float G, float Q,
                    std::tuple<float, float, float> &a,
                    std::tuple<float, float, float> &b);

  static void allpass(float F_s, float f_0, float G, float Q,
                      std::tuple<float, float, float> &a,
                      std::tuple<float, float, float> &b);

  static void peaking(float F_s, float f_0, float G, float Q,
                      std::tuple<float, float, float> &a,
                      std::tuple<float, float, float> &b);

  static void lowshelf(float F_s, float f_0, float G, float S,
                       std::tuple<float, float, float> &a,
                       std::tuple<float, float, float> &b);

  static void highshelf(float F_s, float f_0, float G, float S,
                        std::tuple<float, float, float> &a,
                        std::tuple<float, float, float> &b);

  WEB_AUDIO_PRIVATE : std::shared_ptr<AudioParam> frequency_;
  std::shared_ptr<AudioParam> detune_;
  std::shared_ptr<AudioParam> Q_;
  std::shared_ptr<AudioParam> gain_;
  BiquadFilterType type_ = BiquadFilterType::eLowpass;
  // y[n], y[n-1], y[n-2]
  std::vector<std::tuple<float, float, float>> y_;
  // x[n], x[n-1], x[n-2]
  std::vector<std::tuple<float, float, float>> x_;
};
} // namespace web_audio