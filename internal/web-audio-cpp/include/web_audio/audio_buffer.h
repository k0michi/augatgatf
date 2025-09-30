#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "audio_buffer_options.h"
#include "dom_exception.h"

namespace web_audio {
class AudioBuffer {
public:
  AudioBuffer(const AudioBufferOptions &options);
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

private:
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

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio {
AudioBuffer::AudioBuffer(const AudioBufferOptions &options) {
  if (options.numberOfChannels == 0) {
    throw DOMException("AudioBuffer: numberOfChannels must be at least 1",
                       "NotSupportedError");
  }

  if (options.length == 0) {
    throw DOMException("AudioBuffer: length must be at least 1",
                       "NotSupportedError");
  }

  if (options.sampleRate <= 0) {
    throw DOMException("AudioBuffer: sampleRate must be greater than 0",
                       "NotSupportedError");
  }

  numberOfChannels_ = options.numberOfChannels;
  length_ = options.length;
  sampleRate_ = options.sampleRate;

  channelData_.resize(numberOfChannels_);
  for (auto &channel : channelData_) {
    channel.resize(length_, 0.0f);
  }
}

float AudioBuffer::getSampleRate() const { return sampleRate_; }

std::uint32_t AudioBuffer::getLength() const { return length_; }

double AudioBuffer::getDuration() const {
  return static_cast<double>(length_) / static_cast<double>(sampleRate_);
}

std::uint32_t AudioBuffer::getNumberOfChannels() const {
  return numberOfChannels_;
}

std::vector<float> &AudioBuffer::getChannelData(std::uint32_t channel) & {
  if (channel >= numberOfChannels_) {
    throw DOMException("AudioBuffer: channel index out of range",
                       "IndexSizeError");
  }

  return channelData_[channel];
}

const std::vector<float> &
AudioBuffer::getChannelData(std::uint32_t channel) const & {
  if (channel >= numberOfChannels_) {
    throw DOMException("AudioBuffer: channel index out of range",
                       "IndexSizeError");
  }

  return channelData_[channel];
}

std::vector<float> &&AudioBuffer::getChannelData(std::uint32_t channel) && {
  if (channel >= numberOfChannels_) {
    throw DOMException("AudioBuffer: channel index out of range",
                       "IndexSizeError");
  }

  return std::move(channelData_[channel]);
}

void AudioBuffer::copyFromChannel(std::vector<float> &destination,
                                  std::uint32_t channelNumber,
                                  std::uint32_t bufferOffset) {
  if (channelNumber >= numberOfChannels_) {
    throw DOMException("AudioBuffer: channel index out of range",
                       "IndexSizeError");
  }

  std::uint32_t copyLength =
      std::max(0u, std::min(static_cast<std::uint32_t>(destination.size()),
                            length_ - bufferOffset));

  auto srcBegin = channelData_[channelNumber].begin() + bufferOffset;
  auto srcEnd = srcBegin + copyLength;
  auto dstBegin = destination.begin();
  std::copy(srcBegin, srcEnd, dstBegin);
}

void AudioBuffer::copyToChannel(const std::vector<float> &source,
                                std::uint32_t channelNumber,
                                std::uint32_t bufferOffset) {
  if (channelNumber >= numberOfChannels_) {
    throw DOMException("AudioBuffer: channel index out of range",
                       "IndexSizeError");
  }

  std::uint32_t copyLength =
      std::max(0u, std::min(static_cast<std::uint32_t>(source.size()),
                            length_ - bufferOffset));

  std::copy_n(source.data(), copyLength,
              channelData_[channelNumber].data() + bufferOffset);
}
} // namespace web_audio
#endif