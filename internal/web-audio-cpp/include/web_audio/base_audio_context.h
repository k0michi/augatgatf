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
class BaseAudioContext : public std::enable_shared_from_this<
                             BaseAudioContext> /* : EventTarget */ {
protected:
  BaseAudioContext();

public:
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

  template <typename MessageType> void queueMessage(MessageType &&message) {
    controlMessageQueue_.push(std::forward<MessageType>(message));
  }

protected:
  void initialize();

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
  std::vector<std::shared_ptr<AudioNode>> nodes_;
};
} // namespace web_audio