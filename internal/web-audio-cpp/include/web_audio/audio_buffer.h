#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>

#include "audio_buffer_options.h"
#include "details/common.h"
#include "dom_exception.h"

namespace web_audio {
class AudioBuffer : public std::enable_shared_from_this<AudioBuffer> {
  WEB_AUDIO_PRIVATE : AudioBuffer();

public:
  virtual ~AudioBuffer() noexcept = default;

  float getSampleRate() const;

  std::uint32_t getLength() const;

  double getDuration() const;

  std::uint32_t getNumberOfChannels() const;

  std::vector<float> &getChannelData(std::uint32_t channel) &;

  const std::vector<float> &getChannelData(std::uint32_t channel) const &;

  std::vector<float> &&getChannelData(std::uint32_t channel) &&;

  void copyFromChannel(std::vector<float> &destination,
                       std::uint32_t channelNumber,
                       std::uint32_t bufferOffset = 0);

  void copyToChannel(const std::vector<float> &source,
                     std::uint32_t channelNumber,
                     std::uint32_t bufferOffset = 0);

  static std::shared_ptr<AudioBuffer> create(const AudioBufferOptions &options);

  WEB_AUDIO_PRIVATE :
      // [[number of channels]]
      std::uint32_t numberOfChannels_;
  // [[length]]
  std::uint32_t length_;
  // [[sample rate]]
  float sampleRate_;
  // [[internal data]]
  std::vector<std::vector<float>> channelData_;
};
} // namespace web_audio