#pragma once

#include <optional>

#include "channel_count_mode.hh"
#include "channel_interpretation.hh"

namespace web_audio {
struct AudioNodeOptions {
  std::optional<std::uint32_t> channelCount;
  std::optional<ChannelCountMode> channelCountMode;
  std::optional<ChannelInterpretation> channelInterpretation;
};
} // namespace web_audio