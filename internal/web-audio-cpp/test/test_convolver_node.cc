#include <gtest/gtest.h>

#include <web_audio.hh>

#include "test_helper.hh"

TEST(TestConvolverNode, Offline_Identity) {
  auto context = TestHelper::createOfflineContext();
  auto node = web_audio::ConstantSourceNode::create(context);
  auto convolverNode = web_audio::ConvolverNode::create(context);
  node->getOffset()->setValue(1.0f);
  node->connect(convolverNode);
  convolverNode->connect(context->getDestination());
  auto buffer = web_audio::AudioBuffer::create({
      .numberOfChannels = 1,
      .length = 2,
      .sampleRate = 44100.0f,
  });
  buffer->copyToChannel(std::vector<float>{1.0f, 0.0f}, 0, 0);
  convolverNode->setNormalize(false);
  convolverNode->setBuffer(buffer);

  auto result = TestHelper::renderOffline(context);

  EXPECT_EQ(result->getNumberOfChannels(), 2u);
  EXPECT_EQ(result->getLength(), 128u);
  for (std::uint32_t ch = 0; ch < result->getNumberOfChannels(); ++ch) {
    for (std::uint32_t i = 0; i < result->getLength(); ++i) {
      EXPECT_NEAR(result->getChannelData(ch)[i], 1.0f, 0.01f);
    }
  }
}