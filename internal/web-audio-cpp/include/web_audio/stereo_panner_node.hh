#pragma once

#include "audio_node.hh"
#include "stereo_panner_options.hh"

namespace web_audio {
class StereoPannerNode : public AudioNode {
public:
  // SPEC: constructor (BaseAudioContext context, optional StereoPannerOptions
  // options = {});
  static std::shared_ptr<StereoPannerNode>
  create(std::shared_ptr<BaseAudioContext> context,
         const StereoPannerOptions &options = {});

  // SPEC: readonly attribute AudioParam pan;
  std::shared_ptr<AudioParam> getPan() const;

  std::vector<std::shared_ptr<AudioParam>> getParams() const override;

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  WEB_AUDIO_PRIVATE : std::shared_ptr<AudioParam> pan_;
};
} // namespace web_audio