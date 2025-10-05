#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>

#include "channel_count_mode.h"
#include "channel_interpretation.h"
#include "details/audio_node_input.h"
#include "details/audio_node_output.h"
#include "details/param_collection.h"
#include "details/render_quantum.h"
#include "dom_exception.h"

namespace web_audio::details {
class AudioGraph;
} // namespace web_audio::details

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
  std::shared_ptr<AudioNode> connect(std::shared_ptr<AudioNode> destinationNode,
                                     std::uint32_t output = 0,
                                     std::uint32_t input = 0);
  void connect(std::shared_ptr<AudioParam> destinationParam,
               std::uint32_t output = 0);
  void disconnect();
  void disconnect(std::uint32_t output);
  void disconnect(std::shared_ptr<AudioNode> destinationNode);
  void disconnect(std::shared_ptr<AudioNode> destinationNode,
                  std::uint32_t output);
  void disconnect(std::shared_ptr<AudioNode> destinationNode,
                  std::uint32_t output, std::uint32_t input);
  void disconnect(std::shared_ptr<AudioParam> destinationParam);
  void disconnect(std::shared_ptr<AudioParam> destinationParam,
                  std::uint32_t output);

  std::shared_ptr<BaseAudioContext> getContext() const;

  std::uint32_t getNumberOfInputs() const;

  std::uint32_t getNumberOfOutputs() const;

  std::uint32_t getChannelCount() const;
  virtual void setChannelCount(std::uint32_t channelCount);

  ChannelCountMode getChannelCountMode() const;
  virtual void setChannelCountMode(ChannelCountMode channelCountMode);

  ChannelInterpretation getChannelInterpretation() const;
  virtual void
  setChannelInterpretation(ChannelInterpretation channelInterpretation);

  virtual void process(const std::vector<details::RenderQuantum> &inputs,
                       std::vector<details::RenderQuantum> &outputs,
                       const details::ParamCollection &params) = 0;

  void disconnectInternal(std::size_t index);

  /**
   * Must be called from the create() factory method of each
   * derived class.
   */
  void initialize(std::shared_ptr<BaseAudioContext> context);

protected:
  std::weak_ptr<BaseAudioContext> context_;
  std::uint32_t numberOfInputs_;
  std::uint32_t numberOfOutputs_;
  std::uint32_t channelCount_;
  ChannelCountMode channelCountMode_;
  ChannelInterpretation channelInterpretation_;

  std::vector<details::AudioNodeInput> inputs_;
  std::vector<details::AudioNodeOutput> outputs_;
  std::vector<std::weak_ptr<AudioNode>> inputsIndirect_;

  friend class BaseAudioContext;
  friend class AudioContext;
  friend class OfflineAudioContext;
  friend class details::AudioGraph;
};
} // namespace web_audio