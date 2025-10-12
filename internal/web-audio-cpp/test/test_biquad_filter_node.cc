#include <gtest/gtest.h>
#include <web_audio.hh>

TEST(BiquadFilterNodeTest, Create) {
  auto context = web_audio::AudioContext::create();
  auto node = web_audio::BiquadFilterNode::create(context);
  EXPECT_EQ(node->getType(), web_audio::BiquadFilterType::eLowpass);
  EXPECT_NE(node->getFrequency(), nullptr);
  EXPECT_NE(node->getDetune(), nullptr);
  EXPECT_NE(node->getQ(), nullptr);
  EXPECT_NE(node->getGain(), nullptr);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse) {
  auto context = web_audio::AudioContext::create();
  auto node = web_audio::BiquadFilterNode::create(context);
  std::vector<float> frequencyHz = {100,  200,  400,  800,
                                    1600, 3200, 6400, 12800};
  // From Chrome
  std::vector<float> expectedMagResponse = {1.049, 1.184, 0.940, 0.213,
                                            0.049, 0.012, 0.003, 0.000};
  std::vector<float> expectedPhaseResponse = {-0.270, -0.647, -1.863, -2.693,
                                              -2.940, -3.045, -3.096, -3.123};
  std::vector<float> magResponse(frequencyHz.size());
  std::vector<float> phaseResponse(frequencyHz.size());
  node->getFrequencyResponse(frequencyHz, magResponse, phaseResponse);
  for (std::size_t i = 0; i < frequencyHz.size(); ++i) {
    EXPECT_NEAR(magResponse[i], expectedMagResponse[i], 0.01)
        << "frequencyHz[" << i << "] = " << frequencyHz[i];
    EXPECT_NEAR(phaseResponse[i], expectedPhaseResponse[i], 0.01)
        << "frequencyHz[" << i << "] = " << frequencyHz[i];
  }
}