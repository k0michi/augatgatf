#pragma once

#include <cstdint>
#include <memory>
#include <thread>

#include "audio_buffer.h"
#include "base_audio_context.h"
#include "details/common.h"
#include "details/message_queue.h"
#include "event_handler.h"
#include "offline_audio_context_options.h"
#include "promise.h"

namespace web_audio {
class OfflineAudioContext : public BaseAudioContext {
public:
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

  WEB_AUDIO_PRIVATE :
      // [[rendering started]]
      bool renderingStarted_ = false;
  // [[rendered buffer]]
  std::shared_ptr<AudioBuffer> renderedBuffer_;
  std::uint32_t length_;
  EventHandler *oncomplete_ = nullptr;

  std::unique_ptr<std::thread> renderingThread_;
};
} // namespace web_audio