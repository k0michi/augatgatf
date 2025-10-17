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
  static void fourierTransform(const std::vector<T> &input,
                               std::vector<std::complex<T>> &output) {
    std::vector<std::complex<T>> inputComplex(input.begin(), input.end());
    fourierTransform(inputComplex, output);
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

  template <std::floating_point T>
  static void inverseFourierTransform(const std::vector<T> &input,
                                      std::vector<std::complex<T>> &output) {
    std::vector<std::complex<T>> inputComplex(input.begin(), input.end());
    inverseFourierTransform(inputComplex, output);
  }

  template <std::floating_point T>
  static void convolve(const std::vector<std::complex<T>> &a,
                       const std::vector<std::complex<T>> &b,
                       std::vector<std::complex<T>> &result) {
    if (a.empty() || b.empty()) {
      result.clear();
      return;
    }

    const std::size_t resultSize = a.size() + b.size() - 1;
    std::size_t fftSize = 1;

    while (fftSize < resultSize) {
      fftSize *= 2;
    }

    std::vector<std::complex<T>> aPadded(fftSize, 0);
    std::copy(a.begin(), a.end(), aPadded.begin());

    std::vector<std::complex<T>> bPadded(fftSize, 0);
    std::copy(b.begin(), b.end(), bPadded.begin());

    std::vector<std::complex<T>> aTransformed(fftSize);
    fourierTransform(aPadded, aTransformed);

    std::vector<std::complex<T>> bTransformed(fftSize);
    fourierTransform(bPadded, bTransformed);

    std::vector<std::complex<T>> product(fftSize);

    for (std::size_t i = 0; i < fftSize; ++i) {
      product[i] = aTransformed[i] * bTransformed[i] * static_cast<T>(fftSize);
    }

    std::vector<std::complex<T>> convolved(fftSize);
    inverseFourierTransform(product, convolved);

    result.assign(convolved.begin(), convolved.begin() + resultSize);
  }

  template <std::floating_point T>
  static void convolve(const std::vector<T> &a, const std::vector<T> &b,
                       std::vector<T> &result) {
    if (a.empty() || b.empty()) {
      result.clear();
      return;
    }

    std::vector<std::complex<T>> aComplex(a.begin(), a.end());
    std::vector<std::complex<T>> bComplex(b.begin(), b.end());
    std::vector<std::complex<T>> convolvedComplex;

    convolve(aComplex, bComplex, convolvedComplex);

    result.resize(convolvedComplex.size());
    for (std::size_t i = 0; i < convolvedComplex.size(); ++i) {
      result[i] = convolvedComplex[i].real();
    }
  }
};
} // namespace web_audio::detail