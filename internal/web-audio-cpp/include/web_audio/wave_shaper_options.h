#pragma once

#include <vector>

#include "over_sample_type.h"

namespace web_audio {
struct WaveShaperOptions {
  std::vector<float> curve;
  OverSampleType oversample = OverSampleType::eNone;
};
} // namespace web_audio