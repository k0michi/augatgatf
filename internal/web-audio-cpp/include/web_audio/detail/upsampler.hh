#pragma once

#include <complex>
#include <vector>

#include "common.hh"

namespace web_audio::detail {
class Upsampler {
  WEB_AUDIO_PROTECTED : Upsampler(const std::vector<float> &filterCoefficients,
                                  std::size_t factor, std::size_t blockSize);

public:
  void process(const std::vector<float> &input, std::vector<float> &output);

  WEB_AUDIO_PROTECTED : std::size_t factor_;
  std::size_t filterSize_;
  std::size_t inputBlockSize_;
  std::size_t outputBlockSize_;
  std::size_t fftSize_;

  std::vector<float> filterCoefficients_;
  std::vector<std::complex<float>> fftCoefficientsFrequency_;
  std::vector<std::complex<float>> overlapBuffer_;
};

class Upsampler2x : public Upsampler {
public:
  Upsampler2x(std::size_t blockSize);
};

class Upsampler4x : public Upsampler {
public:
  Upsampler4x(std::size_t blockSize);
};
} // namespace web_audio::detail