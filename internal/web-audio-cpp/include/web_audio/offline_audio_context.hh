#pragma once

#include <cstdint>
#include <memory>

#include "audio_buffer.hh"
#include "base_audio_context.hh"
#include "detail/common.hh"
#include "detail/message_queue.hh"
#include "event_handler.hh"
#include "offline_audio_context_options.hh"
#include "promise.hh"

namespace web_audio {
class OfflineAudioContext : public BaseAudioContext {
public:
  OfflineAudioContext() = default;
  virtual ~OfflineAudioContext() = default;

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

  void process() override;

  WEB_AUDIO_PRIVATE :
      // [[rendering started]]
      bool renderingStarted_ = false;
  // [[rendered buffer]]
  std::shared_ptr<AudioBuffer> renderedBuffer_;
  std::uint32_t length_;
  EventHandler *oncomplete_ = nullptr;
  std::shared_ptr<PromiseInternal<std::shared_ptr<AudioBuffer>>>
      renderingPromiseInternal_;
};
} // namespace web_audio