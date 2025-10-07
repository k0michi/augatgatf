#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <optional>
#include <queue>
#include <thread>

#include "audio_context_state.hh"
#include "audio_destination_node.hh"
#include "audio_listener.hh"
#include "audio_worklet.hh"
#include "detail/audio_graph.hh"
#include "detail/event_queue.hh"
#include "detail/message.hh"
#include "detail/message_queue.hh"
#include "event_handler.hh"
#include "promise.hh"

namespace web_audio {
class AudioNode;

class BaseAudioContext : public std::enable_shared_from_this<
                             BaseAudioContext> /* : EventTarget */ {
protected:
  BaseAudioContext();

public:
  virtual ~BaseAudioContext();

public:
  std::shared_ptr<AudioDestinationNode> getDestination();
  float getSampleRate() const;
  double getCurrentTime() const;
  std::shared_ptr<AudioListener> getListener();
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

  WEB_AUDIO_PROTECTED : void initialize(std::uint32_t numberOfChannels);

  detail::AudioGraph *getAudioGraph();

  std::optional<detail::RenderQuantum> render();

  void run();

  virtual void process();

  WEB_AUDIO_PROTECTED :
      // [[pending promises]]
      std::vector<PromiseBase>
          pendingPromises_;
  // [[rendering thread state]]
  AudioContextState renderThreadState_;
  // [[control thread state]]
  AudioContextState controlThreadState_;
  // [[render quantum size]]
  std::uint32_t renderQuantumSize_;
  // [[current frame]]
  std::atomic<std::uint64_t> currentFrame_{0};

  std::shared_ptr<AudioWorklet> audioWorklet_;
  float sampleRate_;
  EventHandler *onstatechange_ = nullptr;
  std::atomic<double> currentTime_{0.0};

  detail::EventQueue eventQueue_;
  detail::MessageQueue controlMessageQueue_;
  detail::AudioGraph audioGraph_;
  std::unique_ptr<std::thread> renderingThread_;

  friend class AudioNode;
};
} // namespace web_audio