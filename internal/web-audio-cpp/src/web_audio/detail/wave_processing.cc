#include "web_audio/detail/wave_processing.hh"

#include <cmath>
#include <numbers>

namespace web_audio::detail {
void WaveProcessing::fourierTransform(
    const std::vector<std::complex<double>> &input,
    std::vector<std::complex<double>> &output) {
  std::size_t n = input.size();
  if (n <= 1) {
    return;
  }

  std::vector<std::complex<double>> even(n / 2);
  std::vector<std::complex<double>> odd(n / 2);

  for (std::size_t i = 0; i < n / 2; ++i) {
    even[i] = input[i * 2];
    odd[i] = input[i * 2 + 1];
  }

  fourierTransform(even, even);
  fourierTransform(odd, odd);

  output.resize(n);

  const double invN = 1.0 / static_cast<double>(n);

  for (std::size_t i = 0; i < n / 2; ++i) {
    constexpr double kPi = std::numbers::pi;
    std::complex<double> t = std::polar(1.0, -2 * kPi * i * invN) * odd[i];
    output[i] = even[i] + t;
    output[i + n / 2] = even[i] - t;
  }

  for (std::size_t i = 0; i < n; ++i) {
    output[i] /= 2;
  }
}

void WaveProcessing::inverseFourierTransform(
    const std::vector<std::complex<double>> &input,
    std::vector<std::complex<double>> &output) {
  std::size_t n = input.size();
  if (n <= 1) {
    return;
  }

  std::vector<std::complex<double>> even(n / 2);
  std::vector<std::complex<double>> odd(n / 2);

  for (std::size_t i = 0; i < n / 2; ++i) {
    even[i] = input[i * 2];
    odd[i] = input[i * 2 + 1];
  }

  inverseFourierTransform(even, even);
  inverseFourierTransform(odd, odd);

  output.resize(n);

  const double invN = 1.0 / static_cast<double>(n);

  for (std::size_t i = 0; i < n / 2; ++i) {
    constexpr double kPi = std::numbers::pi;
    std::complex<double> t = std::polar(1.0, 2 * kPi * i * invN) * odd[i];
    output[i] = even[i] + t;
    output[i + n / 2] = even[i] - t;
  }
}
} // namespace web_audio::detail
