#include "web_audio/base_audio_context.hh"

namespace web_audio {
BaseAudioContext::BaseAudioContext() {}

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

void BaseAudioContext::initialize() {
  audioWorklet_.reset(new AudioWorklet());
  // TODO

  audioGraph_.initialize(shared_from_this());
}

detail::AudioGraph *BaseAudioContext::getAudioGraph() { return &audioGraph_; }
} // namespace web_audio