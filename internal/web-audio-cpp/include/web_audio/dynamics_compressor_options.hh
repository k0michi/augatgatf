#pragma once

#include "audio_node_options.hh"

namespace web_audio {
struct DynamicsCompressorOptions : public AudioNodeOptions {
  float attack = 0.003f;
  float knee = 30;
  float ratio = 12;
  float release = 0.25;
  float threashold = -24;
};
} // namespace web_audio