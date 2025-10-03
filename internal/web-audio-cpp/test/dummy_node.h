#pragma once

#include "web_audio/audio_node.h"
#include "web_audio/details/param_collection.h"

namespace {
class DummyNode : public web_audio::AudioNode {
private:
  DummyNode() = default;

public:
  static std::shared_ptr<DummyNode>
  create(std::shared_ptr<web_audio::BaseAudioContext> context = nullptr) {
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

  void
  process(const std::vector<web_audio::details::RenderQuantum> &inputBuffer,
          std::vector<web_audio::details::RenderQuantum> &outputBuffer,
          const web_audio::details::ParamCollection &params) override {}
};
} // namespace