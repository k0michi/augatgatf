#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "base_audio_context.hh"
#include "detail/common.hh"
#include "dom_exception.hh"
#include "periodic_wave_options.hh"

namespace web_audio {
class OscillatorNode;

class PeriodicWave : public std::enable_shared_from_this<PeriodicWave> {
  WEB_AUDIO_PRIVATE : PeriodicWave() = default;

public:
  PeriodicWave(const PeriodicWave &) = delete;
  // PeriodicWave(PeriodicWave &&) noexcept;
  virtual ~PeriodicWave() noexcept = default;

  PeriodicWave &operator=(const PeriodicWave &) = delete;
  // PeriodicWave &operator=(PeriodicWave &&) noexcept;

  static std::shared_ptr<PeriodicWave>
  create(std::shared_ptr<BaseAudioContext> context,
         const PeriodicWaveOptions &options = {});

  float getSample(float phase) const;

  WEB_AUDIO_PRIVATE :
      // [[real]]
      std::vector<float>
          real_;
  // [[imag]]
  std::vector<float> imag_;
  // [[normalize]]
  bool normalize_;

  std::vector<float> waveTable_;

  constexpr static std::size_t kTableSize = 8192;

  friend class OscillatorNode;
};
} // namespace web_audio