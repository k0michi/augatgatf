#include "web_audio/periodic_wave.hh"

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
  return wave;
}
} // namespace web_audio