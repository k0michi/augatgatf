#include "test_helper.hh"
#include "web_audio/audio_buffer.hh"
#include "web_audio/audio_buffer_source_node.hh"
#include "web_audio/offline_audio_context.hh"
#include <gtest/gtest.h>

using namespace web_audio;

TEST(AudioBufferSourceNodeTest, CreateAndSetBuffer) {
  auto context = TestHelper::createOfflineContext();
  AudioBufferOptions bufferOptions;
  bufferOptions.numberOfChannels = 2;
  bufferOptions.length = 128;
  bufferOptions.sampleRate = 44100.0f;
  auto buffer = AudioBuffer::create(bufferOptions);

  AudioBufferSourceOptions options;
  options.buffer = buffer;
  auto node = AudioBufferSourceNode::create(context, options);

  EXPECT_EQ(node->getBuffer(), buffer);
}

TEST(AudioBufferSourceNodeTest, PlaybackRateAndDetune) {
  auto context = TestHelper::createOfflineContext();
  AudioBufferSourceOptions options;
  options.playbackRate = 2.0f;
  options.detune = 100.0f;
  auto node = AudioBufferSourceNode::create(context, options);

  EXPECT_FLOAT_EQ(node->getPlaybackRate()->getValue(), 2.0f);
  EXPECT_FLOAT_EQ(node->getDetune()->getValue(), 100.0f);
}

TEST(AudioBufferSourceNodeTest, LoopAttributes) {
  auto context = TestHelper::createOfflineContext();
  AudioBufferSourceOptions options;
  options.loop = true;
  options.loopStart = 1.0;
  options.loopEnd = 2.0;
  auto node = AudioBufferSourceNode::create(context, options);

  EXPECT_TRUE(node->getLoop());
  EXPECT_DOUBLE_EQ(node->getLoopStart(), 1.0);
  EXPECT_DOUBLE_EQ(node->getLoopEnd(), 2.0);

  node->setLoop(false);
  node->setLoopStart(0.5);
  node->setLoopEnd(1.5);
  EXPECT_FALSE(node->getLoop());
  EXPECT_DOUBLE_EQ(node->getLoopStart(), 0.5);
  EXPECT_DOUBLE_EQ(node->getLoopEnd(), 1.5);
}

TEST(AudioBufferSourceNodeTest, StartAndProcess) {
  auto context = TestHelper::createOfflineContext();
  AudioBufferOptions bufferOptions;
  bufferOptions.numberOfChannels = 1;
  bufferOptions.length = 128;
  bufferOptions.sampleRate = 44100.0f;
  auto buffer = AudioBuffer::create(bufferOptions);
  for (std::uint32_t i = 0; i < buffer->getLength(); ++i) {
    buffer->getChannelData(0)[i] = static_cast<float>(i) / buffer->getLength();
  }
  auto node = AudioBufferSourceNode::create(context);
  node->setBuffer(buffer);
  node->connect(context->getDestination());
  node->start();

  auto rendered = TestHelper::renderOffline(context);

  EXPECT_EQ(rendered->getLength(), 128u);

  for (std::uint32_t i = 0; i < rendered->getLength(); ++i) {
    float expected = static_cast<float>(i) / buffer->getLength();
    EXPECT_NEAR(rendered->getChannelData(0)[i], expected, 0.01f);
  }
}
