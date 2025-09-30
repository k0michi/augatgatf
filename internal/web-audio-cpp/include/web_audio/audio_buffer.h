#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>

#include "audio_buffer_options.h"
#include "dom_exception.h"

namespace web_audio {
class AudioBuffer : public std::enable_shared_from_this<AudioBuffer> {
private:
  AudioBuffer();

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
AudioBuffer::AudioBuffer() {}

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

std::shared_ptr<AudioBuffer>
AudioBuffer::create(const AudioBufferOptions &options) {
  auto buffer = std::shared_ptr<AudioBuffer>(new AudioBuffer());

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

  buffer->numberOfChannels_ = options.numberOfChannels;
  buffer->length_ = options.length;
  buffer->sampleRate_ = options.sampleRate;

  buffer->channelData_.resize(buffer->numberOfChannels_);
  for (auto &channel : buffer->channelData_) {
    channel.resize(buffer->length_, 0.0f);
  }

  return buffer;
}
} // namespace web_audio
#endif