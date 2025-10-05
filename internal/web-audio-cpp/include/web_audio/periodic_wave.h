#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "base_audio_context.h"
#include "dom_exception.h"
#include "periodic_wave_options.h"

namespace web_audio {
class PeriodicWave : public std::enable_shared_from_this<PeriodicWave> {
private:
  PeriodicWave() = default;

public:
  PeriodicWave(const PeriodicWave &) = delete;
  // PeriodicWave(PeriodicWave &&) noexcept;
  virtual ~PeriodicWave() noexcept = default;

  PeriodicWave &operator=(const PeriodicWave &) = delete;
  // PeriodicWave &operator=(PeriodicWave &&) noexcept;

  static std::shared_ptr<PeriodicWave>
  create(std::shared_ptr<BaseAudioContext> context,
         const PeriodicWaveOptions &options = {});

private:
  // [[real]]
  std::vector<float> real_;
  // [[imag]]
  std::vector<float> imag_;
  // [[normalize]]
  bool normalize_;
};
} // namespace web_audio