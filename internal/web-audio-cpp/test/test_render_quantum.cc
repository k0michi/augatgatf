#define WEB_AUDIO_IMPLEMENTATION
#include "web_audio/details/render_quantum.h"
#include <gtest/gtest.h>

using namespace web_audio;

TEST(RenderQuantumTest, MixMonoToStereo) {
  details::RenderQuantum rq(1, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.mix(2, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 2);
  EXPECT_EQ(data[0][0], 1.0f);
  EXPECT_EQ(data[1][0], 1.0f);
}

TEST(RenderQuantumTest, MixMonoToQuad) {
  details::RenderQuantum rq(1, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.mix(4, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 4);
  EXPECT_EQ(data[0][0], 1.0f);
  EXPECT_EQ(data[1][0], 1.0f);
  EXPECT_EQ(data[2][0], 0.0f);
  EXPECT_EQ(data[3][0], 0.0f);
}

TEST(RenderQuantumTest, MixMonoTo5_1) {
  details::RenderQuantum rq(1, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.mix(6, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 6);
  EXPECT_EQ(data[0][0], 0.0f);
  EXPECT_EQ(data[1][0], 0.0f);
  EXPECT_EQ(data[2][0], 1.0f);
  EXPECT_EQ(data[3][0], 0.0f);
  EXPECT_EQ(data[4][0], 0.0f);
  EXPECT_EQ(data[5][0], 0.0f);
}

TEST(RenderQuantumTest, MixStereoToQuad) {
  details::RenderQuantum rq(2, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.mix(4, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 4);
  EXPECT_EQ(data[0][0], 1.0f);
  EXPECT_EQ(data[1][0], 0.5f);
  EXPECT_EQ(data[2][0], 0.0f);
  EXPECT_EQ(data[3][0], 0.0f);
}

TEST(RenderQuantumTest, MixStereoTo5_1) {
  details::RenderQuantum rq(2, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.mix(6, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 6);
  EXPECT_EQ(data[0][0], 1.0f);
  EXPECT_EQ(data[1][0], 0.5f);
  EXPECT_EQ(data[2][0], 0.0f);
  EXPECT_EQ(data[3][0], 0.0f);
  EXPECT_EQ(data[4][0], 0.0f);
  EXPECT_EQ(data[5][0], 0.0f);
}

TEST(RenderQuantumTest, MixQuadTo5_1) {
  details::RenderQuantum rq(4, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.getChannelData()[2][0] = 0.25f;
  rq.getChannelData()[3][0] = 0.125f;
  rq.mix(6, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 6);
  EXPECT_EQ(data[0][0], 1.0f);
  EXPECT_EQ(data[1][0], 0.5f);
  EXPECT_EQ(data[2][0], 0);
  EXPECT_EQ(data[3][0], 0);
  EXPECT_EQ(data[4][0], 0.25f);
  EXPECT_EQ(data[5][0], 0.125f);
}

TEST(RenderQuantumTest, MixStereoToMono) {
  details::RenderQuantum rq(2, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.mix(1, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 1);
  EXPECT_NEAR(data[0][0], 0.5f * (1.0f + 0.5f), 0.01f);
}

TEST(RenderQuantumTest, MixQuadToMono) {
  details::RenderQuantum rq(4, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.getChannelData()[2][0] = 0.25f;
  rq.getChannelData()[3][0] = 0.125f;
  rq.mix(1, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 1);
  EXPECT_NEAR(data[0][0], 0.25f * (1.0f + 0.5f + 0.25f + 0.125f), 0.01f);
}

TEST(RenderQuantumTest, Mix5_1ToMono) {
  details::RenderQuantum rq(6, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.getChannelData()[2][0] = 0.25f;
  rq.getChannelData()[3][0] = 0.125f;
  rq.getChannelData()[4][0] = 0.0625f;
  rq.getChannelData()[5][0] = 0.03125f;
  rq.mix(1, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 1);
  EXPECT_NEAR(data[0][0],
              std::sqrtf(0.5f) * (1.0f + 0.5f) + 0.25f +
                  0.5f * (0.0625f + 0.03125f),
              0.01f);
}

TEST(RenderQuantumTest, MixQuadToStereo) {
  details::RenderQuantum rq(4, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.getChannelData()[2][0] = 0.25f;
  rq.getChannelData()[3][0] = 0.125f;
  rq.mix(2, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 2);
  EXPECT_NEAR(data[0][0], 0.5f * (1.0f + 0.25f), 0.01f);
  EXPECT_NEAR(data[1][0], 0.5f * (0.5f + 0.125f), 0.01f);
}

TEST(RenderQuantumTest, Mix5_1ToStereo) {
  details::RenderQuantum rq(6, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.getChannelData()[2][0] = 0.25f;
  rq.getChannelData()[3][0] = 0.125f;
  rq.getChannelData()[4][0] = 0.0625f;
  rq.getChannelData()[5][0] = 0.03125f;
  rq.mix(2, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 2);
  EXPECT_NEAR(data[0][0], 1.0f + std::sqrtf(0.5f) * (0.25f + 0.0625f), 0.01f);
  EXPECT_NEAR(data[1][0], 0.5f + std::sqrtf(0.5f) * (0.25f + 0.03125f), 0.01f);
}

TEST(RenderQuantumTest, Mix5_1ToQuad) {
  details::RenderQuantum rq(6, 128);
  rq.getChannelData()[0][0] = 1.0f;
  rq.getChannelData()[1][0] = 0.5f;
  rq.getChannelData()[2][0] = 0.25f;
  rq.getChannelData()[3][0] = 0.125f;
  rq.getChannelData()[4][0] = 0.0625f;
  rq.getChannelData()[5][0] = 0.03125f;
  rq.mix(4, ChannelInterpretation::eSpeakers);
  auto &data = rq.getChannelData();
  EXPECT_EQ(data.size(), 4);
  EXPECT_NEAR(data[0][0], 1.0f + std::sqrtf(0.5f) * 0.25f, 0.01f);
  EXPECT_NEAR(data[1][0], 0.5f + std::sqrtf(0.5f) * 0.25f, 0.01f);
  EXPECT_NEAR(data[2][0], 0.0625f, 0.01f);
  EXPECT_NEAR(data[3][0], 0.03125f, 0.01f);
}