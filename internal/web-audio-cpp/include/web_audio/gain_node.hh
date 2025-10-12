#pragma once

#include "audio_node.hh"
#include "gain_options.hh"

namespace web_audio {
class GainNode : public AudioNode {
public:
  // SPEC: constructor (BaseAudioContext context, optional GainOptions options
  // = {});
  static std::shared_ptr<GainNode>
  create(std::shared_ptr<BaseAudioContext> context, GainOptions options = {});

  // SPEC: readonly attribute AudioParam gain;
  std::shared_ptr<AudioParam> getGain() const;

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  std::vector<std::shared_ptr<AudioParam>> getParams() const override;

private:
  std::shared_ptr<AudioParam> gain_;
};
} // namespace web_audio