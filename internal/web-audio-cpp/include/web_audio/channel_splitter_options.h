#pragma once

#include "audio_node_options.h"

namespace web_audio {
struct ChannelSplitterOptions : public AudioNodeOptions {
  std::uint32_t numberOfOutputs = 6;
};
} // namespace web_audio