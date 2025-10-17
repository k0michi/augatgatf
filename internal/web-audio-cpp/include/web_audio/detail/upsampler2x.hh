#pragma once

#include <complex>
#include <vector>

#include "common.hh"

namespace web_audio::detail {
class Upsampler2x {
public:
  Upsampler2x(std::size_t blockSize);

  void process(const std::vector<float> &input, std::vector<float> &output);

  WEB_AUDIO_PRIVATE : std::size_t filterSize_;
  std::size_t inputBlockSize_;
  std::size_t outputBlockSize_;
  std::size_t fftSize_;

  std::vector<float> filterCoefficients_;
  std::vector<std::complex<float>> fftCoefficientsFrequency_;
  std::vector<std::complex<float>> overlapBuffer_;
};
} // namespace web_audio::detail