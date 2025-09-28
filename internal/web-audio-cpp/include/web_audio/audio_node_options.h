#pragma once

#include <optional>

#include "channel_count_mode.h"
#include "channel_interpretation.h"

namespace web_audio {
struct AudioNodeOptions {
  std::optional<std::uint32_t> channelCount;
  std::optional<ChannelCountMode> channelCountMode;
  std::optional<ChannelInterpretation> channelInterpretation;
};
} // namespace web_audio