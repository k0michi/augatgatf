#pragma once

#include <complex>
#include <concepts>
#include <numbers>
#include <vector>

namespace web_audio::detail {
class WaveProcessing {
public:
  template <std::floating_point T>
  static void fourierTransform(const std::vector<std::complex<T>> &input,
                               std::vector<std::complex<T>> &output) {
    std::size_t n = input.size();
    if (n <= 1) {
      return;
    }

    std::vector<std::complex<T>> even(n / 2);
    std::vector<std::complex<T>> odd(n / 2);

    for (std::size_t i = 0; i < n / 2; ++i) {
      even[i] = input[i * 2];
      odd[i] = input[i * 2 + 1];
    }

    fourierTransform(even, even);
    fourierTransform(odd, odd);

    output.resize(n);

    const T invN = 1.0 / static_cast<T>(n);

    for (std::size_t i = 0; i < n / 2; ++i) {
      constexpr auto kPi = std::numbers::pi_v<T>;
      std::complex<T> t = std::polar(1.0, -2 * kPi * i * invN) * odd[i];
      output[i] = even[i] + t;
      output[i + n / 2] = even[i] - t;
    }

    for (std::size_t i = 0; i < n; ++i) {
      output[i] /= 2;
    }
  }

  template <std::floating_point T>
  static void inverseFourierTransform(const std::vector<std::complex<T>> &input,
                                      std::vector<std::complex<T>> &output) {
    std::size_t n = input.size();
    if (n <= 1) {
      return;
    }

    std::vector<std::complex<T>> even(n / 2);
    std::vector<std::complex<T>> odd(n / 2);

    for (std::size_t i = 0; i < n / 2; ++i) {
      even[i] = input[i * 2];
      odd[i] = input[i * 2 + 1];
    }

    inverseFourierTransform(even, even);
    inverseFourierTransform(odd, odd);

    output.resize(n);

    const T invN = 1.0 / static_cast<T>(n);

    for (std::size_t i = 0; i < n / 2; ++i) {
      constexpr auto kPi = std::numbers::pi_v<T>;
      std::complex<T> t = std::polar(1.0, 2 * kPi * i * invN) * odd[i];
      output[i] = even[i] + t;
      output[i + n / 2] = even[i] - t;
    }
  }
};
} // namespace web_audio::detail