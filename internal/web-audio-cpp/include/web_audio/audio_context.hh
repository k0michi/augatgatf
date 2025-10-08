#pragma once

#include <memory>

#ifdef WEB_AUDIO_SDL3
#include <SDL3/SDL.h>
#endif

#include "web_audio/audio_context_options.hh"
#include "web_audio/audio_render_capacity.hh"
#include "web_audio/audio_timestamp.hh"
#include "web_audio/base_audio_context.hh"
#include "web_audio/event_handler.hh"
#include "web_audio/promise.hh"

namespace web_audio {
class AudioContext : public BaseAudioContext {
protected:
  AudioContext() = default;

public:
  virtual ~AudioContext() noexcept;

public:
  // SPEC: constructor (optional AudioContextOptions contextOptions = {});
  static std::shared_ptr<AudioContext>
  create(const AudioContextOptions &contextOptions = {});

  // SPEC: readonly attribute double baseLatency;
  double getBaseLatency() const;

  // SPEC: readonly attribute double outputLatency;
  double getOutputLatency() const;

  // SPEC: [SecureContext] readonly attribute (DOMString or AudioSinkInfo)
  // sinkId;
  std::variant<std::string, AudioSinkInfo> getSinkId() const;

  // SPEC: [SecureContext] readonly attribute AudioRenderCapacity
  // renderCapacity;
  std::shared_ptr<AudioRenderCapacity> getRenderCapacity() const;

  // SPEC: attribute EventHandler onsinkchange;
  EventHandler *getOnsinkchange() const;

  void setOnsinkchange(EventHandler *value);

  // SPEC: attribute EventHandler onerror;
  EventHandler *getOnerror() const;

  void setOnerror(EventHandler *value);

  // SPEC: AudioTimestamp getOutputTimestamp ();
  AudioTimestamp getOutputTimestamp() const;

  // SPEC: Promise<undefined> resume ();
  Promise<void> resume();

  // SPEC: Promise<undefined> suspend ();
  Promise<void> suspend();

  // SPEC: Promise<undefined> close ();
  Promise<void> close();

  // SPEC: [SecureContext] Promise<undefined> setSinkId ((DOMString or
  // AudioSinkOptions) sinkId);
  Promise<void>
  setSinkId(const std::variant<std::string, AudioSinkInfo> &sinkId);

  // SPEC: MediaElementAudioSourceNode createMediaElementSource
  // (HTMLMediaElement mediaElement);

  // SPEC: MediaStreamAudioSourceNode createMediaStreamSource (MediaStream
  // mediaStream);

  // SPEC: MediaStreamTrackAudioSourceNode createMediaStreamTrackSource
  // (MediaStreamTrack mediaStreamTrack);

  // SPEC: MediaStreamAudioDestinationNode createMediaStreamDestination ();

#ifdef WEB_AUDIO_SDL3
  static void callback(void *userdata, SDL_AudioStream *stream,
                       int additional_amount, int total_amount);
#endif

private:
  // SPEC: [[sink ID]]
  std::variant<std::string, AudioSinkInfo> sinkId_;

  std::shared_ptr<AudioRenderCapacity> renderCapacity_;
  EventHandler *onsinkchange_ = nullptr;
  EventHandler *onerror_ = nullptr;

#ifdef WEB_AUDIO_SDL3
  SDL_AudioDeviceID deviceId_ = 0;
  SDL_AudioStream *audioStream_ = nullptr;
#endif
};
} // namespace web_audio