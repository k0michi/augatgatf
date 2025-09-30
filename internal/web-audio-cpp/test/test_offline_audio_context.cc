#include <gtest/gtest.h>

#include "web_audio/offline_audio_context.h"

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