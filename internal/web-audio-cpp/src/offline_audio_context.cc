#include "web_audio/offline_audio_context.hh"

namespace web_audio {
OfflineAudioContext::OfflineAudioContext() : BaseAudioContext() {}

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
    this->renderedBuffer_ = AudioBuffer::create(
        AudioBufferOptions{destination_->channelCount_, length_, sampleRate_});
  } catch (const DOMException &e) {
    promise.getInternal()->reject(std::make_exception_ptr(e));
    return std::move(promise);
  }

  auto internal = promise.getInternal();

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
  throw std::runtime_error("Not implemented");
}

Promise<void> OfflineAudioContext::suspend(double suspendTime) {
  // TODO
  throw std::runtime_error("Not implemented");
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
  context->destination_ =
      std::shared_ptr<AudioDestinationNode>(new AudioDestinationNode());
  context->destination_->channelCount_ = options.numberOfChannels;

  // TODO: worklet

  context->length_ = options.length;
  context->sampleRate_ = options.sampleRate;
  return context;
}

std::shared_ptr<OfflineAudioContext>
OfflineAudioContext::create(std::uint32_t numberOfChannels,
                            std::uint32_t length, float sampleRate) {
  return create(
      OfflineAudioContextOptions{numberOfChannels, length, sampleRate});
}
} // namespace web_audio