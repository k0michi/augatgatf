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

  auto promise = context->startRendering();

  bool called = false;
  promise.then([&](std::shared_ptr<web_audio::AudioBuffer> buffer) {
    EXPECT_EQ(buffer->getNumberOfChannels(), 2u);
    EXPECT_EQ(buffer->getLength(), 128u);
    for (std::uint32_t ch = 0; ch < buffer->getNumberOfChannels(); ++ch) {
      auto &&data = buffer->getChannelData(ch);
      for (std::uint32_t i = 0; i < buffer->getLength(); ++i) {
        EXPECT_NEAR(data[i], 1.0f, 0.01f);
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