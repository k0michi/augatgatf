#pragma once

#include "../audio_listener.hh"
#include "../audio_node.hh"
#include "common.hh"

namespace web_audio::detail {
class AudioListenerNode : public AudioNode {
protected:
  AudioListenerNode() = default;

public:
  static std::shared_ptr<AudioListenerNode>
  create(std::shared_ptr<BaseAudioContext> context);

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

public:
  std::shared_ptr<AudioListener> getListener() const;

  WEB_AUDIO_PRIVATE : std::shared_ptr<AudioListener> listener_;
};
} // namespace web_audio::detail
