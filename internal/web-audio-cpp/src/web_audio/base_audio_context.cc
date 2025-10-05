#include "web_audio/base_audio_context.hh"

namespace web_audio {
BaseAudioContext::BaseAudioContext() {}

AudioDestinationNode *BaseAudioContext::getDestination() {
  return destination_.get();
}

float BaseAudioContext::getSampleRate() const { return sampleRate_; }

double BaseAudioContext::getCurrentTime() const { return currentTime_.load(); }

AudioListener *BaseAudioContext::getListener() { return listener_.get(); }

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
  destination_.reset(new AudioDestinationNode());
  destination_->context_ = shared_from_this();
  listener_.reset(new AudioListener());
  // TODO
  audioWorklet_.reset(new AudioWorklet());
  // TODO
}
} // namespace web_audio