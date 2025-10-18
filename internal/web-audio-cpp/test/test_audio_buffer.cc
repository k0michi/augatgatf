#include <gtest/gtest.h>

#include "web_audio/audio_buffer.hh"

using namespace web_audio;

TEST(TestAudioBuffer, Create) {
  AudioBufferOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto buffer = AudioBuffer::create(options);
  EXPECT_EQ(buffer->getNumberOfChannels(), 2u);
  EXPECT_EQ(buffer->getLength(), 128u);
  EXPECT_EQ(buffer->getSampleRate(), 44100.0f);
  EXPECT_NEAR(buffer->getDuration(), 128.0 / 44100.0, 0.0001f);
}

TEST(TestAudioBuffer, CreateInvalid) {
  AudioBufferOptions options;

  options.numberOfChannels = 0;
  options.length = 128;
  options.sampleRate = 44100.0f;
  EXPECT_THROW(AudioBuffer::create(options), DOMException);

  options.numberOfChannels = 2;
  options.length = 0;
  options.sampleRate = 44100.0f;
  EXPECT_THROW(AudioBuffer::create(options), DOMException);

  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 0.0f;
  EXPECT_THROW(AudioBuffer::create(options), DOMException);
}

TEST(TestAudioBuffer, CopyFromToChannel) {
  AudioBufferOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto buffer = AudioBuffer::create(options);
  auto &ch0 = buffer->getChannelData(0);
  auto &ch1 = buffer->getChannelData(1);

  for (std::uint32_t i = 0; i < buffer->getLength(); ++i) {
    ch0[i] = static_cast<float>(i) / static_cast<float>(buffer->getLength());
    ch1[i] =
        static_cast<float>(i) / static_cast<float>(buffer->getLength()) * 2.0f;
  }

  std::vector<float> dest(128, 0.0f);
  buffer->copyFromChannel(dest, 0, 0);
  for (std::uint32_t i = 0; i < buffer->getLength(); ++i) {
    EXPECT_NEAR(dest[i], ch0[i], 0.0001f);
  }
}

TEST(TestAudioBuffer, CopyFromChannelOutOfRange) {
  AudioBufferOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto buffer = AudioBuffer::create(options);
  std::vector<float> dest(128, 0.0f);
  EXPECT_THROW(buffer->copyFromChannel(dest, 2, 0), DOMException);
}

TEST(TestAudioBuffer, CopyToChannel) {
  AudioBufferOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto buffer = AudioBuffer::create(options);
  std::vector<float> src(128, 0.0f);
  for (std::uint32_t i = 0; i < src.size(); ++i) {
    src[i] = static_cast<float>(i) / static_cast<float>(src.size());
  }

  buffer->copyToChannel(src, 1, 0);
  auto &ch1 = buffer->getChannelData(1);
  for (std::uint32_t i = 0; i < buffer->getLength(); ++i) {
    EXPECT_NEAR(src[i], ch1[i], 0.0001f);
  }
}

TEST(TestAudioBuffer, CopyToChannelOutOfRange) {
  AudioBufferOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto buffer = AudioBuffer::create(options);
  std::vector<float> src(128, 0.0f);

  EXPECT_THROW(buffer->copyToChannel(src, 2, 0), DOMException);
}

TEST(TestAudioBuffer, GetChannelData) {
  AudioBufferOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto buffer = AudioBuffer::create(options);
  auto &ch0 = buffer->getChannelData(0);
  auto &ch1 = buffer->getChannelData(1);

  EXPECT_EQ(ch0, std::vector<float>(128, 0.0f));
  EXPECT_EQ(ch1, std::vector<float>(128, 0.0f));
}

TEST(TestAudioBuffer, GetChannelDataOutOfRange) {
  AudioBufferOptions options;
  options.numberOfChannels = 2;
  options.length = 128;
  options.sampleRate = 44100.0f;

  auto buffer = AudioBuffer::create(options);
  EXPECT_THROW(buffer->getChannelData(2), DOMException);
}