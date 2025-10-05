#pragma once

#include <optional>

#include "audio_node_options.hh"
#include "biquad_filter_type.hh"

namespace web_audio {
struct BiquadFilterOptions : public AudioNodeOptions {
  BiquadFilterType type = BiquadFilterType::eLowpass;
  float Q = 1;
  float detune = 0;
  float frequency = 350;
  float gain = 0;
};
} // namespace web_audio