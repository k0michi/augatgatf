#include <gtest/gtest.h>

#include "web_audio/details/render_quantum.hh"

using namespace web_audio;

TEST(RenderQuantumTest, MixMonoToStereo) {
  details::RenderQuantum rq(1, 128);
  rq[0][0] = 1.0f;
  rq.mix(2, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 2);
  EXPECT_EQ(rq[0][0], 1.0f);
  EXPECT_EQ(rq[1][0], 1.0f);
}

TEST(RenderQuantumTest, MixMonoToQuad) {
  details::RenderQuantum rq(1, 128);
  rq[0][0] = 1.0f;
  rq.mix(4, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 4);
  EXPECT_EQ(rq[0][0], 1.0f);
  EXPECT_EQ(rq[1][0], 1.0f);
  EXPECT_EQ(rq[2][0], 0.0f);
  EXPECT_EQ(rq[3][0], 0.0f);
}

TEST(RenderQuantumTest, MixMonoTo5_1) {
  details::RenderQuantum rq(1, 128);
  rq[0][0] = 1.0f;
  rq.mix(6, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 6);
  EXPECT_EQ(rq[0][0], 0.0f);
  EXPECT_EQ(rq[1][0], 0.0f);
  EXPECT_EQ(rq[2][0], 1.0f);
  EXPECT_EQ(rq[3][0], 0.0f);
  EXPECT_EQ(rq[4][0], 0.0f);
  EXPECT_EQ(rq[5][0], 0.0f);
}

TEST(RenderQuantumTest, MixStereoToQuad) {
  details::RenderQuantum rq(2, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq.mix(4, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 4);
  EXPECT_EQ(rq[0][0], 1.0f);
  EXPECT_EQ(rq[1][0], 0.5f);
  EXPECT_EQ(rq[2][0], 0.0f);
  EXPECT_EQ(rq[3][0], 0.0f);
}

TEST(RenderQuantumTest, MixStereoTo5_1) {
  details::RenderQuantum rq(2, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq.mix(6, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 6);
  EXPECT_EQ(rq[0][0], 1.0f);
  EXPECT_EQ(rq[1][0], 0.5f);
  EXPECT_EQ(rq[2][0], 0.0f);
  EXPECT_EQ(rq[3][0], 0.0f);
  EXPECT_EQ(rq[4][0], 0.0f);
  EXPECT_EQ(rq[5][0], 0.0f);
}

TEST(RenderQuantumTest, MixQuadTo5_1) {
  details::RenderQuantum rq(4, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq[2][0] = 0.25f;
  rq[3][0] = 0.125f;
  rq.mix(6, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 6);
  EXPECT_EQ(rq[0][0], 1.0f);
  EXPECT_EQ(rq[1][0], 0.5f);
  EXPECT_EQ(rq[2][0], 0);
  EXPECT_EQ(rq[3][0], 0);
  EXPECT_EQ(rq[4][0], 0.25f);
  EXPECT_EQ(rq[5][0], 0.125f);
}

TEST(RenderQuantumTest, MixStereoToMono) {
  details::RenderQuantum rq(2, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq.mix(1, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 1);
  EXPECT_NEAR(rq[0][0], 0.5f * (1.0f + 0.5f), 0.01f);
}

TEST(RenderQuantumTest, MixQuadToMono) {
  details::RenderQuantum rq(4, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq[2][0] = 0.25f;
  rq[3][0] = 0.125f;
  rq.mix(1, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 1);
  EXPECT_NEAR(rq[0][0], 0.25f * (1.0f + 0.5f + 0.25f + 0.125f), 0.01f);
}

TEST(RenderQuantumTest, Mix5_1ToMono) {
  details::RenderQuantum rq(6, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq[2][0] = 0.25f;
  rq[3][0] = 0.125f;
  rq[4][0] = 0.0625f;
  rq[5][0] = 0.03125f;
  rq.mix(1, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 1);
  EXPECT_NEAR(rq[0][0],
              std::sqrt(0.5f) * (1.0f + 0.5f) + 0.25f +
                  0.5f * (0.0625f + 0.03125f),
              0.01f);
}

TEST(RenderQuantumTest, MixQuadToStereo) {
  details::RenderQuantum rq(4, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq[2][0] = 0.25f;
  rq[3][0] = 0.125f;
  rq.mix(2, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 2);
  EXPECT_NEAR(rq[0][0], 0.5f * (1.0f + 0.25f), 0.01f);
  EXPECT_NEAR(rq[1][0], 0.5f * (0.5f + 0.125f), 0.01f);
}

TEST(RenderQuantumTest, Mix5_1ToStereo) {
  details::RenderQuantum rq(6, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq[2][0] = 0.25f;
  rq[3][0] = 0.125f;
  rq[4][0] = 0.0625f;
  rq[5][0] = 0.03125f;
  rq.mix(2, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 2);
  EXPECT_NEAR(rq[0][0], 1.0f + std::sqrt(0.5f) * (0.25f + 0.0625f), 0.01f);
  EXPECT_NEAR(rq[1][0], 0.5f + std::sqrt(0.5f) * (0.25f + 0.03125f), 0.01f);
}

TEST(RenderQuantumTest, Mix5_1ToQuad) {
  details::RenderQuantum rq(6, 128);
  rq[0][0] = 1.0f;
  rq[1][0] = 0.5f;
  rq[2][0] = 0.25f;
  rq[3][0] = 0.125f;
  rq[4][0] = 0.0625f;
  rq[5][0] = 0.03125f;
  rq.mix(4, ChannelInterpretation::eSpeakers);
  EXPECT_EQ(rq.getNumberOfChannels(), 4);
  EXPECT_NEAR(rq[0][0], 1.0f + std::sqrt(0.5f) * 0.25f, 0.01f);
  EXPECT_NEAR(rq[1][0], 0.5f + std::sqrt(0.5f) * 0.25f, 0.01f);
  EXPECT_NEAR(rq[2][0], 0.0625f, 0.01f);
  EXPECT_NEAR(rq[3][0], 0.03125f, 0.01f);
}