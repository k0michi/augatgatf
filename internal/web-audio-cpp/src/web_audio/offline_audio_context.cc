#include "web_audio/offline_audio_context.hh"

namespace web_audio {
OfflineAudioContext::~OfflineAudioContext() noexcept {
  if (renderingThread_ && renderingThread_->joinable()) {
    renderingThread_->join();
  }
}

Promise<std::shared_ptr<AudioBuffer>> OfflineAudioContext::startRendering() {
  // SPEC: If this's relevant global object's associated Document is not fully
  // active then return a promise rejected with "InvalidStateError"
  // DOMException.

  // SPEC: If the [[rendering started]] slot on the OfflineAudioContext is true,
  // return a rejected promise with InvalidStateError, and abort these steps.
  if (renderingStarted_) {
    Promise<std::shared_ptr<AudioBuffer>> promise(eventQueue_);
    promise.getInternal()->reject(std::make_exception_ptr(DOMException(
        "OfflineAudioContext: startRendering has already been called",
        "InvalidStateError")));
    return std::move(promise);
  }

  // SPEC: Set the [[rendering started]] slot of the OfflineAudioContext to
  // true.
  renderingStarted_ = true;

  // SPEC: Let promise be a new promise.
  Promise<std::shared_ptr<AudioBuffer>> promise(eventQueue_);

  // SPEC: Create a new AudioBuffer, with a number of channels, length and
  // sample rate equal respectively to the numberOfChannels, length and
  // sampleRate values passed to this instance’s constructor in the
  // contextOptions parameter. Assign this buffer to an internal slot [[rendered
  // buffer]] in the OfflineAudioContext.
  try {
    this->renderedBuffer_ = AudioBuffer::create(AudioBufferOptions{
        audioGraph_.getDestinationNode()->channelCount_, length_, sampleRate_});
  } catch (const DOMException &e) {
    // SPEC: If an exception was thrown during the preceding AudioBuffer
    // constructor call, reject promise with this exception.
    promise.getInternal()->reject(std::make_exception_ptr(e));
    return std::move(promise);
  }

  // SPEC: Otherwise, in the case that the buffer was successfully constructed,
  // begin offline rendering.
  auto internal = promise.getInternal();

  renderingThread_ = std::make_unique<std::thread>([this, internal]() {
    this->renderThreadState_ = AudioContextState::eRunning;

    while (currentFrame_.load() < length_) {
      auto frame = currentFrame_.load();
      auto rendered = this->render();

      for (std::uint32_t ch = 0;
           ch < this->renderedBuffer_->getNumberOfChannels(); ++ch) {
        auto &channelData = this->renderedBuffer_->getChannelData(ch);
        auto &renderedChannelData = (*rendered)[ch];

        std::copy(renderedChannelData.begin(), renderedChannelData.end(),
                  channelData.begin() + frame);
      }
    }

    this->controlThreadState_ = AudioContextState::eClosed;
    internal->resolve(this->renderedBuffer_);
  });

  // SPEC: Append promise to [[pending promises]].

  // SPEC: Return promise.
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
  context->initialize(options.numberOfChannels);

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