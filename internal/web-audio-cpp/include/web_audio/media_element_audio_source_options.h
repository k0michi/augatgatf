#pragma once

#include <memory>

namespace web_audio {
// HTMLMediaElement
class MediaElement;

struct IIRFilterOptions {
  std::shared_ptr<MediaElement> mediaElement;
};
} // namespace web_audio