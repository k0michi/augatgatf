#pragma once

#include <optional>
#include <vector>

#include "periodic_wave_constraints.hh"

namespace web_audio {
struct PeriodicWaveOptions : public PeriodicWaveConstraints {
  std::optional<std::vector<float>> real;
  std::optional<std::vector<float>> imag;
};
} // namespace web_audio