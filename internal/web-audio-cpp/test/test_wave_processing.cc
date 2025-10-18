
#include <gtest/gtest.h>

#include <web_audio.hh>

#include <numbers>

TEST(TestWaveProcessing, FourierTransformAndInverse) {
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

TEST(TestWaveProcessing, FourierTransformAndInverse_1) {
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

TEST(TestWaveProcessing, Convolve) {
  std::vector<std::complex<double>> a = {{1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}};
  std::vector<std::complex<double>> b = {{0.0, 0.0}, {1.0, 0.0}, {0.5, 0.0}};
  std::vector<std::complex<double>> result;

  web_audio::detail::WaveProcessing::convolve(a, b, result);

  std::vector<std::complex<double>> expected(a.size() + b.size() - 1, 0.0);

  for (std::size_t i = 0; i < a.size(); ++i) {
    for (std::size_t j = 0; j < b.size(); ++j) {
      expected[i + j] += a[i] * b[j];
    }
  }

  ASSERT_EQ(expected.size(), result.size());
  for (std::size_t i = 0; i < expected.size(); ++i) {
    EXPECT_NEAR(expected[i].real(), result[i].real(), 1e-6);
    EXPECT_NEAR(expected[i].imag(), result[i].imag(), 1e-6);
  }
}