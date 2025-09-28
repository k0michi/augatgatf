#pragma once

#include <memory>

namespace web_audio {
class MediaStreamTrack;

struct MediaStreamTrackAudioSourceOptions {
  std::shared_ptr<MediaStreamTrack> mediaStreamTrack;
};
} // namespace web_audio