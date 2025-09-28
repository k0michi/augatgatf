#pragma once

#include <any>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "audio_node_options.h"

namespace web_audio {
struct AudioWorkletNodeOptions : public AudioNodeOptions {
  std::uint32_t numberOfInputs = 1;
  std::uint32_t numberOfOutputs = 1;
  std::vector<std::uint32_t> outputChannelCount;
  std::unordered_map<std::string, double> parameterData;
  std::any processorOptions;
};
} // namespace web_audio