#pragma once

#include <memory>

namespace web_audio {
// HTMLMediaElement
class MediaStream;

struct MediaStreamAudioSourceOptions {
  std::shared_ptr<MediaStream> mediaStream;
};
} // namespace web_audio