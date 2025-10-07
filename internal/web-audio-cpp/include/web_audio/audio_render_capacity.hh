#pragma once

#include "audio_render_capacity_options.hh"
#include "event_handler.hh"

namespace web_audio {
class AudioRenderCapacity /* : EventTarget */ {
  // SPEC: undefined start(optional AudioRenderCapacityOptions options = {});
  void start(
      const AudioRenderCapacityOptions &options = AudioRenderCapacityOptions{});

  // SPEC: undefined stop();
  void stop();

  // SPEC: attribute EventHandler onupdate;
  EventHandler *getOnupdate() const;

  void setOnupdate(EventHandler *value);

private:
  EventHandler *onupdate_ = nullptr;
};
} // namespace web_audio