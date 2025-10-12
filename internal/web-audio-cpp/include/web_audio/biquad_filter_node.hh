#pragma once

#include <tuple>

#include "audio_node.hh"
#include "biquad_filter_options.hh"
#include <complex>

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

  /**
   * \f[
   * H(z) = \frac{\frac{b_0}{a_0} + \frac{b_1}{a_0}z^{-1} +
   \frac{b_2}{a_0}z^{-2}} {1+\frac{a_1}{a_0}z^{-1}+\frac{a_2}{a_0}z^{-2}}
   * \f]
   */
  void transferFrequency(const std::tuple<float, float, float> &a,
                         const std::tuple<float, float, float> &b,
                         const std::complex<float> &z, std::complex<float> &H);

  static void computeIntermediate(float F_s, float f_0, float G, float Q,
                                  float &A, float &omega_0, float &alpha_Q,
                                  float &alpha_Q_dB, float &S, float &alpha_S);

  static void computeCoefficients(BiquadFilterType type, float F_s, float f_0,
                                  float G, float Q,
                                  std::tuple<float, float, float> &a,
                                  std::tuple<float, float, float> &b);

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
  // a0, a1, a2
  std::tuple<float, float, float> a_;
  // b0, b1, b2
  std::tuple<float, float, float> b_;
};
} // namespace web_audio