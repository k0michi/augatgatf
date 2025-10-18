#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "audio_node.hh"
#include "detail/downsampler.hh"
#include "detail/upsampler.hh"
#include "wave_shaper_options.hh"

namespace web_audio {
class WaveShaperNode : public AudioNode {
public:
  // SPEC: constructor (BaseAudioContext context, optional WaveShaperOptions
  // options = {});
  static std::shared_ptr<WaveShaperNode>
  create(std::shared_ptr<BaseAudioContext> context,
         WaveShaperOptions options = {});

  // SPEC: attribute Float32Array? curve;
  std::optional<std::vector<float>> getCurve() const;

  void setCurve(const std::optional<std::vector<float>> &curve);

  // SPEC: attribute OverSampleType oversample;
  OverSampleType getOversample() const;

  void setOversample(OverSampleType oversample);

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  WEB_AUDIO_PRIVATE : WaveShaperNode() = default;

  std::optional<std::vector<float>> curve_;
  OverSampleType oversample_ = OverSampleType::eNone;
  std::unique_ptr<detail::Upsampler> upsampler_;
  std::unique_ptr<detail::Downsampler> downsampler_;
};
} // namespace web_audio
