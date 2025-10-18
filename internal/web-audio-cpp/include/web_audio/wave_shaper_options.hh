#pragma once

#include <optional>
#include <vector>

#include "audio_node_options.hh"
#include "over_sample_type.hh"

namespace web_audio {
struct WaveShaperOptions : public AudioNodeOptions {
  std::optional<std::vector<float>> curve;
  OverSampleType oversample = OverSampleType::eNone;
};
} // namespace web_audio