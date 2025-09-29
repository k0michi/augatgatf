#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "base_audio_context.h"
#include "dom_exception.h"
#include "periodic_wave_options.h"

namespace web_audio {
class PeriodicWave {
public:
  PeriodicWave(std::shared_ptr<BaseAudioContext> context,
               const PeriodicWaveOptions &options = {});

  PeriodicWave(const PeriodicWave &) = delete;
  // PeriodicWave(PeriodicWave &&) noexcept;
  virtual ~PeriodicWave() noexcept = default;

  PeriodicWave &operator=(const PeriodicWave &) = delete;
  // PeriodicWave &operator=(PeriodicWave &&) noexcept;

private:
  // [[real]]
  std::vector<float> real_;
  // [[imag]]
  std::vector<float> imag_;
  // [[normalize]]
  bool normalize_;
};
} // namespace web_audio

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio {
PeriodicWave::PeriodicWave(std::shared_ptr<BaseAudioContext> context,
                           const PeriodicWaveOptions &options) {
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

    real_ = *options.real;
    imag_ = *options.imag;
  } else if (options.real.has_value()) {
    if (options.real->size() < 2) {
      throw DOMException("PeriodicWave: real must have at least two elements",
                         "IndexSizeError");
    }

    real_ = *options.real;
    imag_.resize(real_.size(), 0);
  } else if (options.imag.has_value()) {
    if (options.imag->size() < 2) {
      throw DOMException("PeriodicWave: imag must have at least two elements",
                         "IndexSizeError");
    }

    imag_ = *options.imag;
    real_.resize(imag_.size(), 0);
  } else {
    real_ = {0, 0};
    imag_ = {0, 1};
  }

  real_[0] = 0;
  imag_[0] = 0;
  normalize_ = !options.disableNormalization;
}
} // namespace web_audio
#endif // WEB_AUDIO_IMPLEMENTATION