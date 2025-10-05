#pragma once

#include "web_audio.h"

namespace {
class DummyNode : public web_audio::AudioNode {
private:
  DummyNode() = default;

public:
  static std::shared_ptr<DummyNode>
  create(std::shared_ptr<web_audio::BaseAudioContext> context = nullptr) {
    auto node = std::shared_ptr<DummyNode>(new DummyNode());
    node->initialize(context);

    node->numberOfInputs_ = 2;
    node->numberOfOutputs_ = 2;
    node->channelCount_ = 2;
    node->channelCountMode_ = web_audio::ChannelCountMode::eMax;
    node->channelInterpretation_ = web_audio::ChannelInterpretation::eSpeakers;

    node->param_ = web_audio::AudioParam::create(
        node, 0.0f, -std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        web_audio::AutomationRate::eARate, true);
    node->param1_ = web_audio::AudioParam::create(
        node, 1.0f, -std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        web_audio::AutomationRate::eARate, true);

    return node;
  }

  void
  process(const std::vector<web_audio::details::RenderQuantum> &inputBuffer,
          std::vector<web_audio::details::RenderQuantum> &outputBuffer,
          const web_audio::details::ParamCollection &params) override {}

public:
  std::shared_ptr<web_audio::AudioParam> param_;
  std::shared_ptr<web_audio::AudioParam> param1_;
};
} // namespace