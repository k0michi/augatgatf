#include "web_audio/delay_node.h"

#include "web_audio/base_audio_context.h"

namespace web_audio {
std::shared_ptr<DelayNode>
DelayNode::create(std::shared_ptr<BaseAudioContext> context,
                  const DelayOptions &options) {
  auto node = std::shared_ptr<DelayNode>(new DelayNode());

  if (context) {
    node->context_ = context;
  }

  if (!(options.maxDelayTime >= 0 && options.maxDelayTime <= 180)) {
    throw DOMException(
        "DelayNode: maxDelayTime must be in the range 0 to 180 seconds",
        "NotSupportedError");
  }

  node->maxDelayTime_ = options.maxDelayTime;
  node->delayTime_ = std::shared_ptr<AudioParam>(new AudioParam());

  node->numberOfInputs_ = 1;
  node->numberOfOutputs_ = 1;
  node->channelCount_ = options.channelCount.value_or(2);
  node->channelCountMode_ =
      options.channelCountMode.value_or(ChannelCountMode::eMax);
  node->channelInterpretation_ =
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers);

  auto quantums = static_cast<std::uint32_t>(
      std::ceil(context->getSampleRate() * node->maxDelayTime_ /
                static_cast<float>(context->getRenderQuantumSize())) +
      1);
  node->delayBuffers_.resize(
      quantums, details::RenderQuantum(node->channelCount_,
                                       context->getRenderQuantumSize()));

  node->reader_ = DelayNodeReader::create(node);
  node->writer_ = DelayNodeWriter::create(node);

  return node;
}

void DelayNode::process(const std::vector<details::RenderQuantum> &inputs,
                        std::vector<details::RenderQuantum> &outputs,
                        const details::ParamCollection &params) {
  throw std::runtime_error("Not implemented");
}

std::shared_ptr<DelayNode::DelayNodeReader>
DelayNode::DelayNodeReader::create(std::shared_ptr<DelayNode> delayNode) {
  auto node = std::shared_ptr<DelayNodeReader>(new DelayNodeReader());
  node->delayNode_ = delayNode;
  return node;
}

void DelayNode::DelayNodeReader::process(
    const std::vector<details::RenderQuantum> &inputs,
    std::vector<details::RenderQuantum> &outputs,
    const details::ParamCollection &params) {
  auto locked = this->delayNode_.lock();
  auto sampleRate = locked->context_.lock()->getSampleRate();

  for (std::uint32_t channel = 0; channel < outputs[0].getNumberOfChannels();
       ++channel) {
    for (std::uint32_t i = 0; i < outputs[0].getLength(); ++i) {
      auto delayTime = params.getValue(locked->delayTime_, i);

      auto delaySamples = static_cast<std::uint32_t>(delayTime * sampleRate);
      auto currentPosition =
          ((locked->writeIndex_ - 1) *
               locked->context_.lock()->getRenderQuantumSize() +
           i) %
          (locked->delayBuffers_.size() *
           locked->context_.lock()->getRenderQuantumSize());
      auto readPosition = (currentPosition - delaySamples) %
                          (locked->delayBuffers_.size() *
                           locked->context_.lock()->getRenderQuantumSize());
      auto readQuantum =
          readPosition / locked->context_.lock()->getRenderQuantumSize();
      auto readOffset =
          readPosition % locked->context_.lock()->getRenderQuantumSize();
      auto &readBuffer = locked->delayBuffers_[readQuantum];

      outputs[0][channel][i] = readBuffer[channel][readOffset];
    }
  }
}

std::shared_ptr<DelayNode::DelayNodeWriter>
DelayNode::DelayNodeWriter::create(std::shared_ptr<DelayNode> delayNode) {
  auto node = std::shared_ptr<DelayNodeWriter>(new DelayNodeWriter());
  node->delayNode_ = delayNode;
  return node;
}

void DelayNode::DelayNodeWriter::process(
    const std::vector<details::RenderQuantum> &inputs,
    std::vector<details::RenderQuantum> &outputs,
    const details::ParamCollection &params) {
  auto locked = this->delayNode_.lock();
  auto context = locked->context_.lock();

  locked->delayBuffers_[locked->writeIndex_] = inputs[0];
  locked->writeIndex_ =
      (locked->writeIndex_ + 1) % locked->delayBuffers_.size();
}
} // namespace web_audio