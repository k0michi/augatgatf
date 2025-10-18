#include <gtest/gtest.h>

#include <web_audio.hh>

#include "test_helper.hh"

TEST(ConvolverTest, Identity) {
  std::vector<float> impulseResponse = {1.0f, 0.0f, 0.0f, 0.0f};
  constexpr std::size_t blockSize = 8;
  web_audio::detail::Convolver<float> convolver(impulseResponse, blockSize);
  std::vector<float> input = {0.5f,  -0.5f,  0.25f, -0.25f,
                              0.75f, -0.75f, 1.0f,  -1.0f};
  std::vector<float> output;
  convolver.process(input, output);
  ASSERT_EQ(output.size(), blockSize);
  for (std::size_t i = 0; i < blockSize; ++i) {
    EXPECT_NEAR(output[i], input[i], 1e-6f);
  }
}