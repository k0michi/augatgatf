#include "web_audio/audio_context.hh"

namespace web_audio {
std::shared_ptr<AudioContext>
AudioContext::create(const AudioContextOptions &contextOptions) {
  auto context = std::shared_ptr<AudioContext>(new AudioContext());
  auto channels = 2;
  context->initialize(channels);
  // SPEC: Set a [[control thread state]] to suspended on context.
  context->controlThreadState_ = AudioContextState::eSuspended;
  // SPEC: Set a [[rendering thread state]] to suspended on context.
  context->renderThreadState_ = AudioContextState::eSuspended;

  context->sampleRate_ = contextOptions.sampleRate.value_or(44100.0f);
  context->renderQuantumSize_ = 128;

#ifdef WEB_AUDIO_BACKEND_SDL3
  SDL_AudioSpec spec;
  spec.format = SDL_AUDIO_F32;
  spec.channels = channels;
  spec.freq = static_cast<int>(context->sampleRate_);

  // TODO: Sink
  context->deviceId_ =
      SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

  if (context->deviceId_ == 0) {
    throw DOMException("AudioContext: Failed to open audio device",
                       "NotSupportedError");
  }

  context->audioStream_ = SDL_CreateAudioStream(nullptr, &spec);

  if (!context->audioStream_) {
    SDL_CloseAudioDevice(context->deviceId_);
    throw DOMException("AudioContext: Failed to create audio stream",
                       "NotSupportedError");
  }

  if (!SDL_BindAudioStream(context->deviceId_, context->audioStream_)) {
    SDL_CloseAudioDevice(context->deviceId_);
    SDL_DestroyAudioStream(context->audioStream_);
    throw DOMException("AudioContext: Failed to bind audio stream",
                       "NotSupportedError");
  }

  SDL_SetAudioStreamGetCallback(context->audioStream_, callback, context.get());
#endif

  context->renderCapacity_ = std::make_shared<AudioRenderCapacity>();
  context->sinkId_ = std::string("");
  context->controlMessageQueue_.push(detail::MessageBeginRendering{});
  return context;
}

AudioContext::~AudioContext() noexcept {
  // TODO: Fix synchronization
#ifdef WEB_AUDIO_BACKEND_SDL3
  SDL_SetAudioStreamGetCallback(audioStream_, nullptr, nullptr);

  if (audioStream_) {
    SDL_DestroyAudioStream(audioStream_);
    audioStream_ = nullptr;
  }

  if (deviceId_ != 0) {
    SDL_CloseAudioDevice(deviceId_);
    deviceId_ = 0;
  }
#endif
}

double AudioContext::getBaseLatency() const {
  // TODO
  return 0.0;
}

double AudioContext::getOutputLatency() const {
  // TODO
  return 0.0;
}

std::variant<std::string, AudioSinkInfo> AudioContext::getSinkId() const {
  return sinkId_;
}

std::shared_ptr<AudioRenderCapacity> AudioContext::getRenderCapacity() const {
  return renderCapacity_;
}

EventHandler *AudioContext::getOnsinkchange() const { return onsinkchange_; }

void AudioContext::setOnsinkchange(EventHandler *value) {
  onsinkchange_ = value;
}

EventHandler *AudioContext::getOnerror() const { return onerror_; }

void AudioContext::setOnerror(EventHandler *value) { onerror_ = value; }

AudioTimestamp AudioContext::getOutputTimestamp() const {
  // TODO
  return AudioTimestamp{};
}

Promise<void> AudioContext::resume() {
  // TODO
  throw std::runtime_error("Not implemented");
}

Promise<void> AudioContext::suspend() {
  // TODO
  throw std::runtime_error("Not implemented");
}

Promise<void> AudioContext::close() {
  // TODO
  throw std::runtime_error("Not implemented");
}

Promise<void> AudioContext::setSinkId(
    const std::variant<std::string, AudioSinkInfo> &sinkId) {
  // TODO
  throw std::runtime_error("Not implemented");
}

#ifdef WEB_AUDIO_BACKEND_SDL3
void AudioContext::callback(void *userdata, SDL_AudioStream *stream,
                            int additional_amount, int total_amount) {
  auto context = static_cast<AudioContext *>(userdata);

  auto ceilDiv = [](std::uint32_t a, std::uint32_t b) {
    return (a + b - 1) / b;
  };

  auto quanta =
      ceilDiv(additional_amount,
              sizeof(float) * context->getDestination()->getChannelCount() *
                  context->getRenderQuantumSize());

  for (std::uint32_t i = 0; i < quanta; ++i) {
    auto rendered = context->render();

    if (rendered) {
      auto data = rendered->getInterleaved();
      SDL_PutAudioStreamData(stream, data.data(), data.size() * sizeof(float));
    }
  }
}
#endif
} // namespace web_audio