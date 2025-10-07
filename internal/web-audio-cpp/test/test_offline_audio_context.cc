#include <gtest/gtest.h>

#include "web_audio/offline_audio_context.hh"

using namespace web_audio;

TEST(OfflineAudioContextTest, Create) {
  OfflineAudioContextOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto context = OfflineAudioContext::create(options);
  EXPECT_EQ(context->getLength(), 128u);
  EXPECT_EQ(context->getSampleRate(), 44100.0f);
  EXPECT_EQ(context->getDestination()->getChannelCount(), 2u);
  EXPECT_EQ(context->getState(), AudioContextState::eSuspended);
}

TEST(OfflineAudioContextTest, Render) {
  OfflineAudioContextOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto context = OfflineAudioContext::create(options);
  context->renderThreadState_ = AudioContextState::eRunning;
  auto rendered = context->render();
  EXPECT_EQ(rendered.has_value(), true);
  EXPECT_EQ(rendered->getNumberOfChannels(), 2u);
  EXPECT_EQ(rendered->getLength(), 128u);
  EXPECT_NEAR(context->getCurrentTime(), 128.0 / 44100.0, 0.01);
  EXPECT_EQ(context->currentFrame_.load(), 128u);
}

TEST(OfflineAudioContextTest, StartRendering) {
  OfflineAudioContextOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto context = OfflineAudioContext::create(options);
  auto promise = context->startRendering();
  bool called = false;
  promise.then([&](std::shared_ptr<AudioBuffer> buffer) {
    EXPECT_EQ(buffer->getNumberOfChannels(), 2u);
    EXPECT_EQ(buffer->getLength(), 128u);
    EXPECT_NEAR(context->getCurrentTime(), 128.0 / 44100.0, 0.01);
    EXPECT_EQ(context->currentFrame_.load(), 128u);
    EXPECT_EQ(context->getState(), AudioContextState::eClosed);
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