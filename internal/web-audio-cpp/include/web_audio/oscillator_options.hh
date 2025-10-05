#pragma once

#include <memory>

#include "audio_node_options.hh"
#include "oscillator_type.hh"

namespace web_audio {
class PeriodicWave;

struct OscillatorOptions : public AudioNodeOptions {
  OscillatorType type = OscillatorType::eSine;
  float frequency = 440;
  float detune = 0;
  std::shared_ptr<PeriodicWave> periodicWave;
};
} // namespace web_audio