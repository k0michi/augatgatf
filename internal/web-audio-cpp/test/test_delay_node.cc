#include <gtest/gtest.h>

#include "web_audio.h"

namespace {
std::shared_ptr<web_audio::OfflineAudioContext> createOfflineContext() {
  return web_audio::OfflineAudioContext::create(2, 128, 44100.0f);
}
} // namespace

TEST(DelayNodeTest, Create) {
  auto context = createOfflineContext();
  auto delayNode = web_audio::DelayNode::create(context);
  EXPECT_NE(delayNode, nullptr);
}

TEST(DelayNodeTest, Delay1) {
  auto context = createOfflineContext();
  auto delayNode = web_audio::DelayNode::create(context);
  std::vector<web_audio::details::RenderQuantum> inputs(
      1, web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
  inputs[0][0][0] = 1.0f;
  std::vector<web_audio::details::RenderQuantum> outputs(
      1, web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
  web_audio::details::ParamCollection params;
  params.setValue(delayNode->getDelayTime(), 1.1f / context->getSampleRate());
  delayNode->writer_->process(inputs, outputs, params);
  delayNode->reader_->process(inputs, outputs, params);
  EXPECT_FLOAT_EQ(outputs[0][0][0], 0.0f);
  EXPECT_FLOAT_EQ(outputs[0][0][1], 1.0f);
}

TEST(DelayNodeTest, Delay2) {
  auto context = createOfflineContext();
  auto delayNode = web_audio::DelayNode::create(context);
  std::vector<web_audio::details::RenderQuantum> inputs(
      1, web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
  inputs[0][0][0] = 1.0f;
  std::vector<web_audio::details::RenderQuantum> outputs(
      1, web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
  web_audio::details::ParamCollection params;
  params.setValue(delayNode->getDelayTime(), 2.1f / context->getSampleRate());
  delayNode->writer_->process(inputs, outputs, params);
  delayNode->reader_->process(inputs, outputs, params);
  EXPECT_FLOAT_EQ(outputs[0][0][0], 0.0f);
  EXPECT_FLOAT_EQ(outputs[0][0][2], 1.0f);
}

TEST(DelayNodeTest, Delay1Quantum) {
  auto context = createOfflineContext();
  auto delayNode = web_audio::DelayNode::create(context);
  std::vector<web_audio::details::RenderQuantum> inputs(
      2, web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
  inputs[0][0][0] = 1.0f;
  std::vector<web_audio::details::RenderQuantum> outputs(
      2, web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
  web_audio::details::ParamCollection params;
  params.setValue(delayNode->getDelayTime(),
                  (context->getRenderQuantumSize() + 0.1f) /
                      context->getSampleRate());

  for (int32_t i = 0; i < 2; ++i) {
    std::vector<web_audio::details::RenderQuantum> in(
        1,
        web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
    std::vector<web_audio::details::RenderQuantum> out(
        1,
        web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
    in[0] = inputs[i];

    delayNode->writer_->process(in, out, params);
    delayNode->reader_->process(in, out, params);

    outputs[i] = out[0];
  }

  EXPECT_FLOAT_EQ(outputs[0][0][0], 0.0f);
  EXPECT_FLOAT_EQ(outputs[1][0][0], 1.0f);
}

TEST(DelayNodeTest, DelayChannel) {
  auto context = createOfflineContext();
  auto delayNode = web_audio::DelayNode::create(context);
  std::vector<web_audio::details::RenderQuantum> inputs(
      1, web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
  inputs[0][1][0] = 1.0f;
  std::vector<web_audio::details::RenderQuantum> outputs(
      1, web_audio::details::RenderQuantum(2, context->getRenderQuantumSize()));
  web_audio::details::ParamCollection params;
  params.setValue(delayNode->getDelayTime(), 1.1f / context->getSampleRate());
  delayNode->writer_->process(inputs, outputs, params);
  delayNode->reader_->process(inputs, outputs, params);
  EXPECT_FLOAT_EQ(outputs[0][1][0], 0.0f);
  EXPECT_FLOAT_EQ(outputs[0][1][1], 1.0f);
}