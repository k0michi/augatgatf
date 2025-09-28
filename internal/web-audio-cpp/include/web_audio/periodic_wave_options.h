#pragma once

#include <vector>

#include "periodic_wave_constraints.h"

namespace web_audio {
struct PeriodicWaveOptions : public PeriodicWaveConstraints {
  std::vector<float> real;
  std::vector<float> imag;
};
} // namespace web_audio