#pragma once

#include "web_audio/audio_node.h"

namespace {
class DummyNode : public web_audio::AudioNode {
private:
  DummyNode() = default;

public:
  static std::shared_ptr<DummyNode>
  create(std::shared_ptr<web_audio::BaseAudioContext> context = nullptr);
};

std::shared_ptr<DummyNode>
DummyNode::create(std::shared_ptr<web_audio::BaseAudioContext> context) {
  auto node = std::shared_ptr<DummyNode>(new DummyNode());

  if (context) {
    node->context_ = context;
  }
  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = 2;
  node->channelCountMode_ = web_audio::ChannelCountMode::eMax;
  node->channelInterpretation_ = web_audio::ChannelInterpretation::eSpeakers;

  return node;
}
} // namespace