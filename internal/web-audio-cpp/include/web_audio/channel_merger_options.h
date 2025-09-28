#pragma once

#include "audio_node_options.h"

namespace web_audio {
struct ChannelMergerOptions : public AudioNodeOptions {
  std::uint32_t numberOfInputs = 6;
};
} // namespace web_audio