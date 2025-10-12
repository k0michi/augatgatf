#pragma once

#include <complex>

#include "audio_node.hh"
#include "iir_filter_options.hh"

namespace web_audio {
class IIRFilterNode : public AudioNode {
public:
  // SPEC: constructor (BaseAudioContext context, IIRFilterOptions options);
  static std::shared_ptr<IIRFilterNode>
  create(std::shared_ptr<BaseAudioContext> context, IIRFilterOptions options);

  // SPEC: undefined getFrequencyResponse (Float32Array frequencyHz,
  // Float32Array magResponse, Float32Array phaseResponse);
  void getFrequencyResponse(const std::vector<float> &frequencyHz,
                            std::vector<float> &magResponse,
                            std::vector<float> &phaseResponse);

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  WEB_AUDIO_PRIVATE : void transferTime(const std::vector<double> &a,
                                        const std::vector<double> &b,
                                        const std::vector<float> &x,
                                        std::vector<double> &y);

  void transferFrequency(const std::vector<double> &a,
                         const std::vector<double> &b,
                         const std::complex<double> &z,
                         std::complex<double> &H);

  WEB_AUDIO_PRIVATE : std::vector<double> feedforward_;
  std::vector<double> feedback_;

  // x[n], x[n-1], x[n-2], ...
  std::vector<std::vector<float>> x_;
  // y[n], y[n-1], y[n-2], ...
  std::vector<std::vector<double>> y_;
};
} // namespace web_audio