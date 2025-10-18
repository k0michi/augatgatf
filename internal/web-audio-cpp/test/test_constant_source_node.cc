#include <gtest/gtest.h>

#include <web_audio.hh>

#include "test_helper.hh"

TEST(TestConstantSourceNode, Create) {
  auto context = TestHelper::createOfflineContext();
  auto node = web_audio::ConstantSourceNode::create(context);
  ASSERT_NE(node, nullptr);
  ASSERT_NE(node->getOffset(), nullptr);
  EXPECT_EQ(node->getNumberOfInputs(), 0u);
  EXPECT_EQ(node->getNumberOfOutputs(), 1u);
  EXPECT_EQ(node->getChannelCount(), 2u);
  EXPECT_EQ(node->getChannelCountMode(), web_audio::ChannelCountMode::eMax);
  EXPECT_EQ(node->getChannelInterpretation(),
            web_audio::ChannelInterpretation::eSpeakers);
}

TEST(TestConstantSourceNode, Offline) {
  auto context = TestHelper::createOfflineContext();
  auto node = web_audio::ConstantSourceNode::create(context);
  node->getOffset()->setValue(0.5f);
  node->connect(context->getDestination());

  auto promise = context->startRendering();

  bool called = false;
  promise.then([&](std::shared_ptr<web_audio::AudioBuffer> buffer) {
    EXPECT_EQ(buffer->getNumberOfChannels(), 2u);
    EXPECT_EQ(buffer->getLength(), 128u);
    for (std::uint32_t ch = 0; ch < buffer->getNumberOfChannels(); ++ch) {
      auto &&data = buffer->getChannelData(ch);
      for (std::uint32_t i = 0; i < buffer->getLength(); ++i) {
        EXPECT_NEAR(data[i], 0.5f, 0.01f);
      }
    }
    called = true;
  });
  promise.catch_([&](std::exception_ptr exception) {
    called = true;
    FAIL() << "Promise rejected";
  });

  while (!called) {
    context->processEvents();
  }
}