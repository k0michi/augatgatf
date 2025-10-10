#include "web_audio/periodic_wave.hh"

#include <cmath>
#include <numbers>

namespace web_audio {
std::shared_ptr<PeriodicWave>
PeriodicWave::create(std::shared_ptr<BaseAudioContext> context,
                     const PeriodicWaveOptions &options) {
  auto wave = std::shared_ptr<PeriodicWave>(new PeriodicWave());

  if (options.real.has_value() && options.imag.has_value()) {
    if (options.real->size() != options.imag->size()) {
      throw DOMException(
          "PeriodicWave: real and imag must have the same length",
          "IndexSizeError");
    }

    if (options.real->size() < 2 || options.imag->size() < 2) {
      throw DOMException(
          "PeriodicWave: real and imag must have at least two elements",
          "IndexSizeError");
    }

    wave->real_ = *options.real;
    wave->imag_ = *options.imag;
  } else if (options.real.has_value()) {
    if (options.real->size() < 2) {
      throw DOMException("PeriodicWave: real must have at least two elements",
                         "IndexSizeError");
    }

    wave->real_ = *options.real;
    wave->imag_.resize(wave->real_.size(), 0);
  } else if (options.imag.has_value()) {
    if (options.imag->size() < 2) {
      throw DOMException("PeriodicWave: imag must have at least two elements",
                         "IndexSizeError");
    }

    wave->imag_ = *options.imag;
    wave->real_.resize(wave->imag_.size(), 0);
  } else {
    wave->real_ = {0, 0};
    wave->imag_ = {0, 1};
  }

  wave->real_[0] = 0;
  wave->imag_[0] = 0;
  wave->normalize_ = !options.disableNormalization;

  wave->waveTable_.resize(wave->kTableSize);

  for (std::uint32_t t = 0; t < wave->kTableSize; ++t) {
    float phase = static_cast<float>(t) / wave->kTableSize;

    for (std::size_t i = 0; i < wave->real_.size(); ++i) {
      float theta = 2.0f * static_cast<float>(std::numbers::pi) * phase * i;
      wave->waveTable_[t] +=
          wave->real_[i] * std::cos(theta) + wave->imag_[i] * std::sin(theta);
    }
  }

  if (wave->normalize_) {
    float maxAmplitude = 0.0f;

    for (const auto &sample : wave->waveTable_) {
      maxAmplitude = std::max(maxAmplitude, std::abs(sample));
    }

    if (maxAmplitude > 0.0f) {
      for (auto &sample : wave->waveTable_) {
        sample /= maxAmplitude;
      }
    }
  }

  return wave;
}

float PeriodicWave::getSample(float phase) const {
  if (phase < 0.0f || phase >= 1.0f) {
    phase = phase - std::floor(phase);
  }

  // linear interpolation
  float index = phase * kTableSize;
  std::size_t index0 = static_cast<std::size_t>(std::floor(index)) % kTableSize;
  std::size_t index1 = (index0 + 1) % kTableSize;
  float frac = index - std::floor(index);
  return (1.0f - frac) * waveTable_[index0] + frac * waveTable_[index1];
}
} // namespace web_audio