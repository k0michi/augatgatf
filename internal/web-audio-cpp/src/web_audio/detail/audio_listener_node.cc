#include "web_audio/detail/audio_listener_node.hh"

namespace web_audio::detail {
std::shared_ptr<AudioListenerNode>
AudioListenerNode::create(std::shared_ptr<BaseAudioContext> context) {
  auto node = std::shared_ptr<AudioListenerNode>(new AudioListenerNode());
  node->initialize(context);
  node->listener_ = AudioListener::create(context);
  return node;
}

void AudioListenerNode::process(
    const std::vector<detail::RenderQuantum> &inputs,
    std::vector<detail::RenderQuantum> &outputs,
    const detail::ParamCollection &params) {
  // No operation
}

std::shared_ptr<AudioListener> AudioListenerNode::getListener() const {
  return listener_;
}
} // namespace web_audio::detail
