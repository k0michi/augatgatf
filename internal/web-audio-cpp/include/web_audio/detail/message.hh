#pragma once

#include <variant>

namespace web_audio {
class AudioScheduledSourceNode;
}

namespace web_audio::detail {
struct MessageAudioScheduledSourceNodeStart {
  double when;
  double offset;
  double duration;
  std::shared_ptr<AudioScheduledSourceNode> node;
};

struct MessageAudioScheduledSourceNodeStop {
  double when;
  std::shared_ptr<AudioScheduledSourceNode> node;
};

/**
 * Message to terminate the rendering thread.
 */
struct MessageTerminate {};

/**
 * Message to begin rendering.
 */
struct MessageBeginRendering {};
// TODO: other messages

using Message = std::variant<MessageAudioScheduledSourceNodeStart,
                             MessageAudioScheduledSourceNodeStop,
                             MessageTerminate, MessageBeginRendering>;
} // namespace web_audio::detail