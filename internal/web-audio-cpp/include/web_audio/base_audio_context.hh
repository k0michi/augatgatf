#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <queue>

#include "audio_context_state.hh"
#include "audio_destination_node.hh"
#include "audio_listener.hh"
#include "audio_worklet.hh"
#include "detail/event_queue.hh"
#include "detail/message_queue.hh"
#include "event_handler.hh"
#include "promise.hh"

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

  detail::EventQueue eventQueue_;
  detail::MessageQueue controlMessageQueue_;
  std::vector<std::shared_ptr<AudioNode>> nodes_;
};
} // namespace web_audio