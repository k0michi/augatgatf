#pragma once

#include "audio_scheduled_source_node.hh"
#include "oscillator_options.hh"

namespace web_audio {
class OscillatorNode : public AudioScheduledSourceNode {
protected:
  OscillatorNode() = default;

public:
  // SPEC: constructor (BaseAudioContext context, optional OscillatorOptions
  // options = {});
  static std::shared_ptr<OscillatorNode>
  create(std::shared_ptr<BaseAudioContext> context,
         const OscillatorOptions &options = {});

  // SPEC: attribute OscillatorType type;
  OscillatorType getType() const;

  void setType(OscillatorType type);

  // SPEC: readonly attribute AudioParam frequency;
  std::shared_ptr<AudioParam> getFrequency() const;

  // SPEC: readonly attribute AudioParam detune;
  std::shared_ptr<AudioParam> getDetune() const;

  // SPEC: undefined setPeriodicWave(PeriodicWave periodicWave);
  void setPeriodicWave(std::shared_ptr<PeriodicWave> periodicWave);

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  std::vector<std::shared_ptr<AudioParam>> getParams() const override;

  WEB_AUDIO_PRIVATE : OscillatorType type_;
  std::shared_ptr<AudioParam> frequency_;
  std::shared_ptr<AudioParam> detune_;
  std::shared_ptr<PeriodicWave> periodicWave_;
  float phase_ = 0;
};
} // namespace web_audio
