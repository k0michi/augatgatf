#pragma once

#include <cstdint>
#include <memory>
#include <thread>

#include "audio_buffer.h"
#include "base_audio_context.h"
#include "details/message_queue.h"
#include "event_handler.h"
#include "offline_audio_context_options.h"
#include "promise.h"

namespace web_audio {
class OfflineAudioContext : public BaseAudioContext {
public:
  // OfflineAudioContext(const OfflineAudioContextOptions &options);
  // OfflineAudioContext(std::uint32_t numberOfChannels, std::uint32_t length,
  //                     float sampleRate);
  OfflineAudioContext();
  virtual ~OfflineAudioContext() noexcept = default;

  Promise<std::shared_ptr<AudioBuffer>> startRendering();
  Promise<void> resume();
  Promise<void> suspend(double suspendTime);
  std::uint32_t getLength() const;
  EventHandler *getOncomplete() const;
  void setOncomplete(EventHandler *value);

  static std::shared_ptr<OfflineAudioContext>
  create(const OfflineAudioContextOptions &options);
  static std::shared_ptr<OfflineAudioContext>
  create(std::uint32_t numberOfChannels, std::uint32_t length,
         float sampleRate);

private:
  // [[rendering started]]
  bool renderingStarted_ = false;
  // [[rendered buffer]]
  std::shared_ptr<AudioBuffer> renderedBuffer_;
  std::uint32_t length_;
  EventHandler *oncomplete_ = nullptr;

  std::unique_ptr<std::thread> renderingThread_;
};
} // namespace web_audio

namespace web_audio {
OfflineAudioContext::OfflineAudioContext() : BaseAudioContext() {}

// OfflineAudioContext::OfflineAudioContext(
//     const OfflineAudioContextOptions &options)
//     : BaseAudioContext() {
//   if (options.numberOfChannels == 0) {
//     throw DOMException(
//         "OfflineAudioContext: numberOfChannels must be at least 1",
//         "NotSupportedError");
//   }

//   if (options.length == 0) {
//     throw DOMException("OfflineAudioContext: length must be at least 1",
//                        "NotSupportedError");
//   }

//   if (options.sampleRate <= 0) {
//     throw DOMException("OfflineAudioContext: sampleRate must be greater than
//     0",
//                        "NotSupportedError");
//   }

//   this->controlThreadState_ = AudioContextState::eSuspended;
//   this->renderThreadState_ = AudioContextState::eSuspended;
//   // TODO: renderSizehint
//   this->renderQuantumSize_ = 128;
//   this->destination_ = std::make_unique<AudioDestinationNode>();
//   this->destination_->channelCount_ = options.numberOfChannels;

//   // TODO: worklet
// }

// OfflineAudioContext::OfflineAudioContext(std::uint32_t numberOfChannels,
//                                          std::uint32_t length, float
//                                          sampleRate)
//     : OfflineAudioContext(
//           OfflineAudioContextOptions{numberOfChannels, length, sampleRate})
//           {}

Promise<std::shared_ptr<AudioBuffer>> OfflineAudioContext::startRendering() {
  if (renderingStarted_) {
    Promise<std::shared_ptr<AudioBuffer>> promise(eventQueue_);
    promise.getInternal()->reject(std::make_exception_ptr(DOMException(
        "OfflineAudioContext: startRendering has already been called",
        "InvalidStateError")));
    return std::move(promise);
  }

  renderingStarted_ = true;

  Promise<std::shared_ptr<AudioBuffer>> promise(eventQueue_);

  try {
    this->renderedBuffer_ = std::make_shared<AudioBuffer>(
        AudioBufferOptions{destination_->channelCount_, length_, sampleRate_});
  } catch (const DOMException &e) {
    promise.getInternal()->reject(std::make_exception_ptr(e));
    return std::move(promise);
  }

  auto internal = promise->getInternal();

  renderingThread_ = std::make_unique<std::thread>([this, internal]() {
    {
      details::MessageQueue qRendering;
      this->controlMessageQueue_.swap(qRendering);
      while (true) {
        auto msg = qRendering.pop();
        // TODO: process event
      }

      // TODO: Process the BaseAudioContext's associated task queue.

      // Process a render quantum.
      if (this->renderThreadState_ != AudioContextState::eRunning) {
        return;
      }
    }
  });

  // 8. Append promise to [[pending promises]].

  return std::move(promise);
}

Promise<void> OfflineAudioContext::resume() {
  // TODO
}

Promise<void> OfflineAudioContext::suspend(double suspendTime) {
  // TODO
}

std::uint32_t OfflineAudioContext::getLength() const { return length_; }

EventHandler *OfflineAudioContext::getOncomplete() const { return oncomplete_; }

void OfflineAudioContext::setOncomplete(EventHandler *value) {
  oncomplete_ = value;
}

std::shared_ptr<OfflineAudioContext>
OfflineAudioContext::create(const OfflineAudioContextOptions &options) {
  auto context =
      std::shared_ptr<OfflineAudioContext>(new OfflineAudioContext());

  if (options.numberOfChannels == 0) {
    throw DOMException(
        "OfflineAudioContext: numberOfChannels must be at least 1",
        "NotSupportedError");
  }

  if (options.length == 0) {
    throw DOMException("OfflineAudioContext: length must be at least 1",
                       "NotSupportedError");
  }

  if (options.sampleRate <= 0) {
    throw DOMException(
        "OfflineAudioContext: sampleRate must be greater than 0 ",
        "NotSupportedError");
  }

  context->controlThreadState_ = AudioContextState::eSuspended;
  context->renderThreadState_ = AudioContextState::eSuspended;
  // TODO: renderSizehint
  context->renderQuantumSize_ = 128;
  context->destination_ = std::make_unique<AudioDestinationNode>();
  context->destination_->channelCount_ = options.numberOfChannels;

  // TODO: worklet

  context->length_ = options.length;
  context->sampleRate_ = options.sampleRate;
  return context;
}
} // namespace web_audio