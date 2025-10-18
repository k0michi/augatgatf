#include <gtest/gtest.h>

#include <web_audio.hh>

#include "test_helper.hh"

TEST(WaveShaperNodeTest, Offline) {
  auto context = TestHelper::createOfflineContext();
  auto node = web_audio::ConstantSourceNode::create(context);
  auto waveShaperNode = web_audio::WaveShaperNode::create(context);
  node->getOffset()->setValue(0);
  node->connect(waveShaperNode);
  waveShaperNode->connect(context->getDestination());
  waveShaperNode->setCurve(std::vector<float>{
      1.0f,
      1.0f,
      1.0f,
  });

  auto result = TestHelper::renderOffline(context);

  EXPECT_EQ(result->getNumberOfChannels(), 2u);
  EXPECT_EQ(result->getLength(), 128u);
  for (std::uint32_t ch = 0; ch < result->getNumberOfChannels(); ++ch) {
    auto &&data = result->getChannelData(ch);
    for (std::uint32_t i = 0; i < result->getLength(); ++i) {
      EXPECT_NEAR(data[i], 1.0f, 0.01f);
    }
  }
}

TEST(WaveShaperNodeTest, Offline_Interpolation) {
  auto context = TestHelper::createOfflineContext();
  auto node = web_audio::ConstantSourceNode::create(context);
  auto waveShaperNode = web_audio::WaveShaperNode::create(context);
  node->getOffset()->setValue(0.5);
  node->connect(waveShaperNode);
  waveShaperNode->connect(context->getDestination());
  waveShaperNode->setCurve(std::vector<float>{
      0.0f,
      0.0f,
      0.5f,
  });

  auto result = TestHelper::renderOffline(context);

  EXPECT_EQ(result->getNumberOfChannels(), 2u);
  EXPECT_EQ(result->getLength(), 128u);
  for (std::uint32_t ch = 0; ch < result->getNumberOfChannels(); ++ch) {
    auto &&data = result->getChannelData(ch);
    for (std::uint32_t i = 0; i < result->getLength(); ++i) {
      EXPECT_NEAR(data[i], 0.25f, 0.01f);
    }
  }
}

TEST(WaveShaperNodeTest, Offline_TwoPoints) {
  auto context = TestHelper::createOfflineContext();
  auto node = web_audio::ConstantSourceNode::create(context);
  auto waveShaperNode = web_audio::WaveShaperNode::create(context);
  node->getOffset()->setValue(0);
  node->connect(waveShaperNode);
  waveShaperNode->connect(context->getDestination());
  waveShaperNode->setCurve(std::vector<float>{
      0.0f,
      1.0f,
  });

  auto result = TestHelper::renderOffline(context);

  EXPECT_EQ(result->getNumberOfChannels(), 2u);
  EXPECT_EQ(result->getLength(), 128u);
  for (std::uint32_t ch = 0; ch < result->getNumberOfChannels(); ++ch) {
    auto &&data = result->getChannelData(ch);
    for (std::uint32_t i = 0; i < result->getLength(); ++i) {
      EXPECT_NEAR(data[i], 0.5f, 0.01f);
    }
  }
}