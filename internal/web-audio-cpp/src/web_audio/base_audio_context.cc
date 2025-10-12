#include "web_audio/base_audio_context.hh"

#include <unordered_map>

#include "web_audio/audio_param.hh"
#include "web_audio/audio_scheduled_source_node.hh"
#include "web_audio/offline_audio_context.hh"

namespace web_audio {
BaseAudioContext::BaseAudioContext() {}

BaseAudioContext::~BaseAudioContext() {
  controlMessageQueue_.push(detail::MessageTerminate());

  if (renderingThread_ && renderingThread_->joinable()) {
    renderingThread_->join();
  }
}

std::shared_ptr<AudioDestinationNode> BaseAudioContext::getDestination() {
  return audioGraph_.getDestinationNode();
}

float BaseAudioContext::getSampleRate() const { return sampleRate_; }

double BaseAudioContext::getCurrentTime() const { return currentTime_.load(); }

std::shared_ptr<AudioListener> BaseAudioContext::getListener() {
  return audioGraph_.getListenerNode()->getListener();
}

AudioContextState BaseAudioContext::getState() const {
  return controlThreadState_;
}

std::uint32_t BaseAudioContext::getRenderQuantumSize() const {
  return renderQuantumSize_;
}

AudioWorklet *BaseAudioContext::getAudioWorklet() {
  return audioWorklet_.get();
}

EventHandler *BaseAudioContext::getOnstatechange() const {
  return onstatechange_;
}

void BaseAudioContext::setOnstatechange(EventHandler *value) {
  onstatechange_ = value;
}

void BaseAudioContext::processEvents() { eventQueue_.poll(); }

void BaseAudioContext::initialize(std::uint32_t numberOfChannels) {
  audioWorklet_.reset(new AudioWorklet());
  // TODO

  audioGraph_.initialize(shared_from_this(), numberOfChannels);
  renderingThread_ =
      std::make_unique<std::thread>(&BaseAudioContext::run, this);
}

detail::AudioGraph *BaseAudioContext::getAudioGraph() { return &audioGraph_; }

std::optional<detail::RenderQuantum> BaseAudioContext::render() {
  // SPEC: Process the control message queue.

  // SPEC: Process the BaseAudioContext's associated task queue.

  // SPEC: If the [[rendering thread state]] of the BaseAudioContext is not
  // running, return false.
  if (renderThreadState_ != AudioContextState::eRunning) {
    return std::nullopt;
  }

  // SPEC: Order the AudioNodes of the BaseAudioContext to be processed.
  auto orderedNodes = audioGraph_.orderNodes();
  std::unordered_map<std::shared_ptr<AudioNode>,
                     std::vector<detail::RenderQuantum>>
      nodeResults;
  std::unordered_map<std::shared_ptr<AudioParam>, detail::RenderQuantum>
      paramResults;
  auto currentTime = currentTime_.load();

  for (auto &node : orderedNodes) {
    auto params = node->getParams();

    detail::ParamCollection paramCollection;

    for (auto &param : params) {
      detail::RenderQuantum paramOutput(1, renderQuantumSize_);

      for (auto &input : param->inputs_) {
        if (auto srcNode = input.source.lock()) {
          if (nodeResults.find(srcNode) != nodeResults.end()) {
            // discrete?
            paramOutput.add(nodeResults[srcNode][input.sourceIndex],
                            ChannelInterpretation::eDiscrete);
          }
        }
      }

      param->computeIntrinsicValues(currentTime, paramOutput[0]);

      // SPEC: Queue a control message to set the [[current value]] slot of this
      // AudioParam according to § 1.6.3 Computation of Value.
      // TODO

      paramCollection.setValues(param, paramOutput[0]);
    }

    std::uint32_t inputChannelsMax = 0;

    for (auto &inputNode : node->inputs_) {
      if (auto srcNode = inputNode.source.lock()) {
        if (nodeResults.find(srcNode) != nodeResults.end()) {
          inputChannelsMax = std::max(
              inputChannelsMax, nodeResults[srcNode][inputNode.sourceIndex]
                                    .getNumberOfChannels());
        }
      }
    }

    auto computedNumberOfChannels =
        node->channelCountMode_ == ChannelCountMode::eMax ? inputChannelsMax
        : node->channelCountMode_ == ChannelCountMode::eClampedMax
            ? std::min(node->channelCount_, inputChannelsMax)
            : node->channelCount_;

    std::vector<detail::RenderQuantum> inputs(
        node->getNumberOfInputs(),
        detail::RenderQuantum(computedNumberOfChannels, renderQuantumSize_));

    for (auto &inputNode : node->inputs_) {
      if (auto srcNode = inputNode.source.lock()) {
        if (nodeResults.find(srcNode) != nodeResults.end()) {
          inputs[inputNode.destinationIndex].add(
              nodeResults[srcNode][inputNode.sourceIndex],
              node->channelInterpretation_);
        }
      }
    }

    // process() may change the number of output channels.
    std::vector<detail::RenderQuantum> outputs(
        node->getNumberOfOutputs(),
        detail::RenderQuantum(computedNumberOfChannels, renderQuantumSize_));

    node->process(inputs, outputs, paramCollection);
    nodeResults[node] = std::move(outputs);

    // SPEC: If this AudioNode is an AudioWorkletNode, execute these substeps:
    // TODO
  }

  // SPEC: Atomically perform the following steps:
  currentFrame_ += renderQuantumSize_;
  currentTime_ = static_cast<double>(currentFrame_.load()) / sampleRate_;

  return nodeResults[audioGraph_.getDestinationNode()][0];
}

void BaseAudioContext::run() {
  while (true) {
    std::optional<detail::Message> message;

    if (dynamic_cast<OfflineAudioContext *>(this) &&
        renderThreadState_ == AudioContextState::eRunning) {
      message = controlMessageQueue_.tryPop();
    } else {
      message = controlMessageQueue_.pop();
    }

    if (message) {
      if (std::holds_alternative<detail::MessageTerminate>(*message)) {
        return;
      } else if (std::holds_alternative<detail::MessageBeginRendering>(
                     *message)) {
        this->renderThreadState_ = AudioContextState::eRunning;
      } else if (std::holds_alternative<
                     detail::MessageAudioScheduledSourceNodeStart>(*message)) {
        auto &msg =
            std::get<detail::MessageAudioScheduledSourceNodeStart>(*message);

        msg.node->startTime_ = msg.when;
      } else if (std::holds_alternative<
                     detail::MessageAudioScheduledSourceNodeStop>(*message)) {
        auto &msg =
            std::get<detail::MessageAudioScheduledSourceNodeStop>(*message);

        msg.node->stopTime_ = msg.when;
      } else {
        // TODO
      }
    } else {
      this->process();
    }
  }
}

void BaseAudioContext::process() {}
} // namespace web_audio