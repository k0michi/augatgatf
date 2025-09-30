#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <queue>

#include "audio_context_state.h"
#include "audio_destination_node.h"
#include "audio_listener.h"
#include "audio_worklet.h"
#include "details/event_queue.h"
#include "details/message_queue.h"
#include "event_handler.h"
#include "promise.h"

namespace web_audio {
class BaseAudioContext /* : EventTarget */ {
protected:
  BaseAudioContext();
  virtual ~BaseAudioContext() noexcept = default;

public:
  AudioDestinationNode *getDestination();
  float getSampleRate() const;
  double getCurrentTime() const;
  AudioListener *getListener();
  AudioContextState getState() const;
  std::uint32_t getRenderQuantumSize() const;
  AudioWorklet *getAudioWorklet();
  EventHandler *getOnstatechange() const;
  void setOnstatechange(EventHandler *value);

  // TODO: factories

  /**
   * C.f. wgpuInstanceProcessEvents
   */
  void processEvents();

protected:
  // [[pending promises]]
  std::vector<PromiseBase> pendingPromises_;
  // [[rendering thread state]]
  AudioContextState renderThreadState_;
  // [[control thread state]]
  AudioContextState controlThreadState_;
  // [[render quantum size]]
  std::uint32_t renderQuantumSize_;

  std::shared_ptr<AudioDestinationNode> destination_;
  std::shared_ptr<AudioListener> listener_;
  std::shared_ptr<AudioWorklet> audioWorklet_;
  float sampleRate_;
  EventHandler *onstatechange_ = nullptr;
  std::atomic<double> currentTime_{0.0};

  details::EventQueue eventQueue_;
  details::MessageQueue controlMessageQueue_;
};
} // namespace web_audio

namespace web_audio {
BaseAudioContext::BaseAudioContext() {
  destination_.reset(new AudioDestinationNode());
  destination_->context_ = this;
  listener_.reset(new AudioListener());
  // TODO
  audioWorklet_.reset(new AudioWorklet());
  // TODO
}

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
} // namespace web_audio