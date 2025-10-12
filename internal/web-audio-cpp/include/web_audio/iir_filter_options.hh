#pragma once

#include <vector>

#include "audio_node_options.hh"

namespace web_audio {
struct IIRFilterOptions : public AudioNodeOptions {
  std::vector<double> feedforward;
  std::vector<double> feedback;
};
} // namespace web_audio