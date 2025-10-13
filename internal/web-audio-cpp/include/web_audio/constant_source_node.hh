#pragma once

#include "audio_node.hh"
#include "constant_source_options.hh"

namespace web_audio {
class ConstantSourceNode : public AudioNode {
public:
  // SPEC: constructor (BaseAudioContext context, optional ConstantSourceOptions
  // options = {});
  static std::shared_ptr<ConstantSourceNode>
  create(std::shared_ptr<BaseAudioContext> context,
         const ConstantSourceOptions &options = {});

  // SPEC: readonly attribute AudioParam offset;
  std::shared_ptr<AudioParam> getOffset() const;

  std::vector<std::shared_ptr<AudioParam>> getParams() const override;

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  WEB_AUDIO_PRIVATE : std::shared_ptr<AudioParam> offset_;
};
} // namespace web_audio
