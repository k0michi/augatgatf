
#include <gtest/gtest.h>

#include <web_audio.hh>

#include <numbers>

TEST(WaveProcessingTest, FourierTransformAndInverse) {
  std::vector<std::complex<double>> input = {
      {1.0, 0.0}, {0.0, 1.0}, {-1.0, 0.0}, {0.0, -1.0}};
  std::vector<std::complex<double>> output;
  std::vector<std::complex<double>> reconstructed;

  web_audio::detail::WaveProcessing::fourierTransform(input, output);
  web_audio::detail::WaveProcessing::inverseFourierTransform(output,
                                                             reconstructed);

  ASSERT_EQ(input.size(), reconstructed.size());
  for (std::size_t i = 0; i < input.size(); ++i) {
    EXPECT_NEAR(input[i].real(), reconstructed[i].real(), 1e-6);
    EXPECT_NEAR(input[i].imag(), reconstructed[i].imag(), 1e-6);
  }
}

TEST(WaveProcessingTest, FourierTransformAndInverse_1) {
  std::vector<std::complex<double>> input;

  for (int i = 0; i < 128; ++i) {
    constexpr double kPi = std::numbers::pi;
    input.emplace_back(std::cos(2.0 * kPi * i / 128.0),
                       std::sin(2.0 * kPi * i / 128.0));
  }

  std::vector<std::complex<double>> output;
  std::vector<std::complex<double>> reconstructed;

  web_audio::detail::WaveProcessing::fourierTransform(input, output);
  web_audio::detail::WaveProcessing::inverseFourierTransform(output,
                                                             reconstructed);

  ASSERT_EQ(input.size(), reconstructed.size());
  for (std::size_t i = 0; i < input.size(); ++i) {
    EXPECT_NEAR(input[i].real(), reconstructed[i].real(), 1e-6);
    EXPECT_NEAR(input[i].imag(), reconstructed[i].imag(), 1e-6);
  }
}