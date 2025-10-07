#include "web_audio/audio_render_capacity.hh"

namespace web_audio {
void AudioRenderCapacity::start(const AudioRenderCapacityOptions &options) {
  // TODO
}

void AudioRenderCapacity::stop() {
  // TODO
}

EventHandler *AudioRenderCapacity::getOnupdate() const { return onupdate_; }

void AudioRenderCapacity::setOnupdate(EventHandler *value) {
  onupdate_ = value;
}
} // namespace web_audio