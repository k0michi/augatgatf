#pragma once

#include <cmath>
#include <cstdint>
#include <vector>

#include "../channel_interpretation.hh"
#include "common.hh"

namespace web_audio::details {
class RenderQuantum {
public:
  RenderQuantum(std::uint32_t numberOfChannels, std::uint32_t length);

  std::uint32_t getLength() const;

  std::uint32_t getNumberOfChannels() const;

  std::size_t size() const;

  std::vector<float> &operator[](std::uint32_t channel);

  const std::vector<float> &operator[](std::uint32_t channel) const;

  void mix(std::uint32_t computedNumberOfChannels,
           ChannelInterpretation channelInterpretation);

  WEB_AUDIO_PRIVATE : std::uint32_t length_;
  std::vector<std::vector<float>> channelData_;
};
} // namespace web_audio::details