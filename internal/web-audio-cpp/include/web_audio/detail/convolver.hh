#pragma once

#include <complex>
#include <concepts>
#include <stdexcept>
#include <vector>

#include "math_helper.hh"
#include "wave_processing.hh"

namespace web_audio::detail {
/**
 * Convolver using Overlap-Add method.
 */
template <std::floating_point T> class Convolver final {
public:
  Convolver(std::vector<T> impulseResponse, std::size_t blockSize)
      : blockSize_(blockSize), impulseResponseSize_(impulseResponse.size()) {
    fftSize_ =
        MathHelper::nextPowerOfTwo(blockSize_ + impulseResponse.size() - 1);
    impulseResponse.resize(fftSize_, static_cast<T>(0));
    WaveProcessing::fourierTransform(impulseResponse,
                                     impulseResponseFrequency_);
    overlapBuffer_.resize(impulseResponseSize_ - 1, static_cast<T>(0));
  }

  void process(const std::vector<T> &input, std::vector<T> &output) {
    if (input.size() != blockSize_) {
      throw std::invalid_argument("Input size must be equal to block size.");
    }

    output.resize(blockSize_);

    std::vector<T> inputPadded(fftSize_, static_cast<T>(0));
    std::copy(input.begin(), input.end(), inputPadded.begin());

    std::vector<std::complex<T>> inputFrequency(fftSize_);
    WaveProcessing::fourierTransform(inputPadded, inputFrequency);

    std::vector<std::complex<T>> outputFrequency(fftSize_);

    for (std::size_t i = 0; i < fftSize_; ++i) {
      outputFrequency[i] = inputFrequency[i] * impulseResponseFrequency_[i] *
                           static_cast<T>(fftSize_);
    }

    std::vector<std::complex<T>> outputTime(fftSize_);
    WaveProcessing::inverseFourierTransform(outputFrequency, outputTime);

    for (std::size_t i = 0; i < blockSize_; ++i) {
      T sample =
          outputTime[i].real() +
          (i < overlapBuffer_.size() ? overlapBuffer_[i] : static_cast<T>(0));
      output[i] = sample;
    }

    for (std::size_t i = 0; i + blockSize_ < impulseResponseSize_ - 1; ++i) {
      overlapBuffer_[i] = overlapBuffer_[i + blockSize_];
    }

    for (std::size_t i = 0; i < blockSize_; ++i) {
      overlapBuffer_[overlapBuffer_.size() - blockSize_ + i] =
          static_cast<T>(0);
    }

    for (std::size_t i = 0; i < impulseResponseSize_ - 1; ++i) {
      overlapBuffer_[i] = outputTime[blockSize_ + i].real();
    }

    return;
  }

  WEB_AUDIO_PRIVATE : std::size_t blockSize_;
  std::size_t fftSize_;
  std::size_t impulseResponseSize_;
  std::vector<std::complex<T>> impulseResponseFrequency_;
  std::vector<T> overlapBuffer_;
};
} // namespace web_audio::detail