#pragma once

#include <complex>
#include <vector>

namespace web_audio::detail {
class WaveProcessing {
public:
  static void fourierTransform(const std::vector<std::complex<double>> &input,
                               std::vector<std::complex<double>> &output);

  static void
  inverseFourierTransform(const std::vector<std::complex<double>> &input,
                          std::vector<std::complex<double>> &output);
};
} // namespace web_audio::detail