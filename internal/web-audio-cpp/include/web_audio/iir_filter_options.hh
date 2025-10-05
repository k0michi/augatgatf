#pragma once

#include <vector>

#include "audio_node_options.hh"

namespace web_audio {
struct IIRFilterOptions : public AudioNodeOptions {
  std::vector<float> feedforward;
  std::vector<float> feedback;
};
} // namespace web_audio