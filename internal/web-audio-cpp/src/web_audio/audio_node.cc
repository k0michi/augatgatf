#include "web_audio/audio_node.hh"

#include "web_audio/audio_listener.hh"
#include "web_audio/audio_param.hh"
#include "web_audio/base_audio_context.hh"
#include "web_audio/detail/weak_ptr_helper.hh"

namespace web_audio {
std::shared_ptr<AudioNode>
AudioNode::connect(std::shared_ptr<AudioNode> destinationNode,
                   std::uint32_t output, std::uint32_t input) {
  if (context_.lock() != destinationNode->context_.lock()) {
    throw DOMException("AudioNode: cannot connect nodes from different context",
                       "InvalidAccessError");
  }

  if (output >= numberOfOutputs_) {
    throw DOMException("AudioNode: output index is out of range",
                       "IndexSizeError");
  }

  if (input >= destinationNode->numberOfInputs_) {
    throw DOMException("AudioNode: input index is out of range",
                       "IndexSizeError");
  }

  for (auto &out : outputs_) {
    if (out.sourceIndex == output) {
      if (auto dest = std::get_if<std::weak_ptr<AudioNode>>(&out.destination)) {
        if (auto destNode = dest->lock()) {
          if (destNode == destinationNode && out.destinationIndex == input) {
            // already connected
            return destinationNode;
          }
        }
      }
    }
  }

  outputs_.push_back(detail::AudioNodeOutput{output, destinationNode, input});
  destinationNode->inputs_.push_back(
      detail::AudioNodeInput{shared_from_this(), output, input});

  return destinationNode;
}

void AudioNode::connect(std::shared_ptr<AudioParam> destinationParam,
                        std::uint32_t output) {
  // SPEC: If destinationParam belongs to an AudioNode that belongs to a
  // BaseAudioContext that is different from the BaseAudioContext that has
  // created the AudioNode on which this method was called, an
  // InvalidAccessError MUST be thrown.
  if (context_.lock() != destinationParam->getContext()) {
    throw DOMException("AudioNode: cannot connect nodes from different context",
                       "InvalidAccessError");
  }

  // SPEC: If the parameter is out-of-bounds, an IndexSizeError exception MUST
  // be thrown.
  if (output >= numberOfOutputs_) {
    throw DOMException("AudioNode: output index is out of range",
                       "IndexSizeError");
  }

  for (auto &out : outputs_) {
    if (out.sourceIndex == output) {
      if (auto dest =
              std::get_if<std::weak_ptr<AudioParam>>(&out.destination)) {
        if (auto destParam = dest->lock()) {
          if (destParam == destinationParam) {
            // already connected
            return;
          }
        }
      }
    }
  }

  outputs_.push_back(detail::AudioNodeOutput{output, destinationParam, 0});
  destinationParam->inputs_.push_back(
      detail::AudioNodeInput{shared_from_this(), output, 0});
  auto owner = destinationParam->getOwner();
  owner->inputsIndirect_.push_back(shared_from_this());
}

void AudioNode::disconnect() {
  for (std::size_t i = outputs_.size(); i-- > 0;) {
    disconnectInternal(i);
  }
}

void AudioNode::disconnect(std::uint32_t output) {
  // SPEC: If this parameter is out-of-bounds, an IndexSizeError exception MUST
  // be thrown.
  if (output >= numberOfOutputs_) {
    throw DOMException("AudioNode: output index is out of range",
                       "IndexSizeError");
  }

  for (std::size_t i = outputs_.size(); i-- > 0;) {
    const auto &out = outputs_[i];

    if (out.sourceIndex == output) {
      disconnectInternal(i);
    }
  }
}

void AudioNode::disconnect(std::shared_ptr<AudioNode> destinationNode) {
  bool found = false;

  for (std::size_t i = outputs_.size(); i-- > 0;) {
    const auto &out = outputs_[i];

    if (auto dest = std::get_if<std::weak_ptr<AudioNode>>(&out.destination)) {
      if (auto destNode = dest->lock()) {
        if (destNode == destinationNode) {
          found = true;
          disconnectInternal(i);
        }
      }
    }
  }

  // SPEC: If there is no connection to the destinationNode, an
  // InvalidAccessError exception MUST be thrown.
  if (!found) {
    throw DOMException("AudioNode: no connection to the destinationNode",
                       "InvalidAccessError");
  }
}

void AudioNode::disconnect(std::shared_ptr<AudioNode> destinationNode,
                           std::uint32_t output) {
  bool found = false;

  for (std::size_t i = outputs_.size(); i-- > 0;) {
    auto &out = outputs_[i];

    if (out.sourceIndex == output) {
      if (auto dest = std::get_if<std::weak_ptr<AudioNode>>(&out.destination)) {
        if (auto destNode = dest->lock()) {
          if (destNode == destinationNode) {
            found = true;
            disconnectInternal(i);
          }
        }
      }
    }
  }

  // SPEC: If there is no connection to the destinationNode, an
  // InvalidAccessError exception MUST be thrown.
  if (!found) {
    throw DOMException("AudioNode: no connection to the destinationNode",
                       "InvalidAccessError");
  }
}

void AudioNode::disconnect(std::shared_ptr<AudioNode> destinationNode,
                           std::uint32_t output, std::uint32_t input) {
  // SPEC: If this parameter is out-of-bounds, an IndexSizeError exception MUST
  // be thrown.
  if (output >= numberOfOutputs_) {
    throw DOMException("AudioNode: output index is out of range",
                       "IndexSizeError");
  }

  // SPEC: If this parameter is out-of-bounds, an IndexSizeError exception MUST
  // be thrown.
  if (input >= destinationNode->numberOfInputs_) {
    throw DOMException("AudioNode: input index is out of range",
                       "IndexSizeError");
  }

  bool found = false;

  for (std::size_t i = outputs_.size(); i-- > 0;) {
    auto &out = outputs_[i];
    if (out.sourceIndex == output) {
      if (auto dest = std::get_if<std::weak_ptr<AudioNode>>(&out.destination)) {
        if (auto destNode = dest->lock()) {
          if (destNode == destinationNode && out.destinationIndex == input) {
            found = true;
            disconnectInternal(i);
          }
        }
      }
    }
  }

  // SPEC: If there is no connection to the destinationNode from the given
  // output to the given input, an InvalidAccessError exception MUST be thrown.
  if (!found) {
    throw DOMException("AudioNode: no connection to the destinationNode",
                       "InvalidAccessError");
  }
}

void AudioNode::disconnect(std::shared_ptr<AudioParam> destinationParam) {
  bool found = false;

  for (std::size_t i = outputs_.size(); i-- > 0;) {
    auto &out = outputs_[i];
    if (auto dest = std::get_if<std::weak_ptr<AudioParam>>(&out.destination)) {
      if (auto destParam = dest->lock()) {
        if (destParam == destinationParam) {
          found = true;
          disconnectInternal(i);
        }
      }
    }
  }

  // SPEC: If there is no connection to the destinationParam, an
  // InvalidAccessError exception MUST be thrown.
  if (!found) {
    throw DOMException("AudioNode: no connection to the destinationParam",
                       "InvalidAccessError");
  }
}

void AudioNode::disconnect(std::shared_ptr<AudioParam> destinationParam,
                           std::uint32_t output) {
  // SPEC: If the parameter is out-of-bounds, an IndexSizeError exception MUST
  // be thrown.
  if (output >= numberOfOutputs_) {
    throw DOMException("AudioNode: output index is out of range",
                       "IndexSizeError");
  }

  bool found = false;

  for (std::size_t i = outputs_.size(); i-- > 0;) {
    const auto &out = outputs_[i];

    if (out.sourceIndex == output) {
      if (auto dest =
              std::get_if<std::weak_ptr<AudioParam>>(&out.destination)) {
        if (auto destParam = dest->lock()) {
          if (destParam == destinationParam) {
            found = true;
            disconnectInternal(i);
          }
        }
      }
    }
  }

  // SPEC: If there is no connection to the destinationParam, an
  // InvalidAccessError exception MUST be thrown.
  if (!found) {
    throw DOMException("AudioNode: no connection to the destinationParam",
                       "InvalidAccessError");
  }
}

std::shared_ptr<BaseAudioContext> AudioNode::getContext() const {
  if (auto context = context_.lock()) {
    return context;
  } else {
    throw std::runtime_error("AudioNode: context has expired");
  }
}

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

void AudioNode::initialize(std::shared_ptr<BaseAudioContext> context) {
  // SPEC: Set o’s associated BaseAudioContext to context.
  context_ = context;

  context->audioGraph_.addNode(shared_from_this());
}

void AudioNode::disconnectInternal(std::size_t index) {
  const auto &output = outputs_[index];

  if (auto destNode =
          std::get_if<std::weak_ptr<AudioNode>>(&output.destination)) {
    if (auto sp = destNode->lock()) {
      auto &inputs = sp->inputs_;
      inputs.erase(std::remove(inputs.begin(), inputs.end(),
                               detail::AudioNodeInput{shared_from_this(),
                                                      output.sourceIndex,
                                                      output.destinationIndex}),
                   inputs.end());
      outputs_.erase(outputs_.begin() + index);
    }
  } else if (auto destParam =
                 std::get_if<std::weak_ptr<AudioParam>>(&output.destination)) {
    if (auto sp = destParam->lock()) {
      auto &inputs = sp->inputs_;
      inputs.erase(std::remove(inputs.begin(), inputs.end(),
                               detail::AudioNodeInput{shared_from_this(),
                                                      output.sourceIndex, 0}),
                   inputs.end());
      outputs_.erase(outputs_.begin() + index);

      auto owner = sp->getOwner();
      auto &inputsIndirect = owner->inputsIndirect_;
      inputsIndirect.erase(
          std::remove_if(inputsIndirect.begin(), inputsIndirect.end(),
                         [this](const auto &node) {
                           return detail::WeakPtrHelper::compare(
                                      node, shared_from_this()) == 0;
                         }),
          inputsIndirect.end());
    }
  }
}

std::vector<std::shared_ptr<AudioParam>> AudioNode::getParams() const {
  return {};
}

std::uint32_t AudioNode::computeNumberOfChannels(
    const std::vector<detail::RenderQuantum> &inputs) const {
  if (channelCountMode_ == ChannelCountMode::eMax) {
    std::uint32_t maxChannels = 1;

    for (const auto &input : inputs) {
      maxChannels =
          std::max(maxChannels, static_cast<std::uint32_t>(input.size()));
    }

    return maxChannels;
  } else if (channelCountMode_ == ChannelCountMode::eClampedMax) {
    std::uint32_t maxChannels = 1;

    for (const auto &input : inputs) {
      maxChannels =
          std::max(maxChannels, static_cast<std::uint32_t>(input.size()));
    }

    return std::min(maxChannels, channelCount_);
  } else { // ChannelCountMode::eExplicit
    return channelCount_;
  }
}
} // namespace web_audio