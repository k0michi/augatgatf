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
}