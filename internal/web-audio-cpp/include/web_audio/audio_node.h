#pragma once

#include <cstdint>
#include <memory>

#include "audio_param.h"
#include "channel_count_mode.h"
#include "channel_interpretation.h"
#include "dom_exception.h"

namespace web_audio {
class BaseAudioContext;
class AudioContext;
class OfflineAudioContext;

class AudioNode
    : public std::enable_shared_from_this<AudioNode> /* EventTarget */ {
protected:
  AudioNode() = default;
  virtual ~AudioNode() noexcept = default;

public:
  // TODO: shared_ptr?
  std::shared_ptr<AudioNode> connect(std::shared_ptr<AudioNode> destinationNode,
                                     std::uint32_t output = 0,
                                     std::uint32_t input = 0);
  void connect(std::shared_ptr<AudioParam> destinationParam,
               std::uint32_t output = 0);
  void disconnect();
  void disconnect(std::uint32_t output);
  void disconnect(std::shared_ptr<AudioNode> destinationNode,
                  std::uint32_t output);
  void disconnect(std::shared_ptr<AudioNode> destinationNode,
                  std::uint32_t output, std::uint32_t input);
  void disconnect(std::shared_ptr<AudioParam> destinationParam);
  void disconnect(std::shared_ptr<AudioParam> destinationParam,
                  std::uint32_t output);

  std::weak_ptr<BaseAudioContext> getContext();

  std::uint32_t getNumberOfInputs() const;

  std::uint32_t getNumberOfOutputs() const;

  std::uint32_t getChannelCount() const;
  virtual void setChannelCount(std::uint32_t channelCount);

  ChannelCountMode getChannelCountMode() const;
  virtual void setChannelCountMode(ChannelCountMode channelCountMode);

  ChannelInterpretation getChannelInterpretation() const;
  virtual void
  setChannelInterpretation(ChannelInterpretation channelInterpretation);

protected:
  std::weak_ptr<BaseAudioContext> context_;
  std::uint32_t numberOfInputs_;
  std::uint32_t numberOfOutputs_;
  std::uint32_t channelCount_;
  ChannelCountMode channelCountMode_;
  ChannelInterpretation channelInterpretation_;

  friend class BaseAudioContext;
  friend class AudioContext;
  friend class OfflineAudioContext;
};
} // namespace web_audio

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio {
std::shared_ptr<AudioNode>
AudioNode::connect(std::shared_ptr<AudioNode> destinationNode,
                   std::uint32_t output, std::uint32_t input) {
  // TODO
  return {};
}

void AudioNode::connect(std::shared_ptr<AudioParam> destinationParam,
                        std::uint32_t output) {
  // TODO
}

void AudioNode::disconnect() {
  // TODO
}

void AudioNode::disconnect(std::uint32_t output) {
  // TODO
}

void AudioNode::disconnect(std::shared_ptr<AudioNode> destinationNode,
                           std::uint32_t output) {
  // TODO
}

void AudioNode::disconnect(std::shared_ptr<AudioNode> destinationNode,
                           std::uint32_t output, std::uint32_t input) {
  // TODO
}

void AudioNode::disconnect(std::shared_ptr<AudioParam> destinationParam) {
  // TODO
}

void AudioNode::disconnect(std::shared_ptr<AudioParam> destinationParam,
                           std::uint32_t output) {
  // TODO
}

std::weak_ptr<BaseAudioContext> AudioNode::getContext() { return context_; }

std::uint32_t AudioNode::getNumberOfInputs() const { return numberOfInputs_; }

std::uint32_t AudioNode::getNumberOfOutputs() const { return numberOfOutputs_; }

std::uint32_t AudioNode::getChannelCount() const { return channelCount_; }

void AudioNode::setChannelCount(std::uint32_t channelCount) {
  if (channelCount == 0) {
    throw DOMException("AudioNode: channelCount must be at least 1",
                       "NotSupportedError");
  }

  channelCount_ = channelCount;
}

ChannelCountMode AudioNode::getChannelCountMode() const {
  return channelCountMode_;
}

void AudioNode::setChannelCountMode(ChannelCountMode channelCountMode) {
  channelCountMode_ = channelCountMode;
}

ChannelInterpretation AudioNode::getChannelInterpretation() const {
  return channelInterpretation_;
}

void AudioNode::setChannelInterpretation(
    ChannelInterpretation channelInterpretation) {
  channelInterpretation_ = channelInterpretation;
}
} // namespace web_audio
#endif