#pragma once

#include "audio_node_options.hh"

namespace web_audio {
struct GainOptions : public AudioNodeOptions {
  float gain = 1;
};
} // namespace web_audio