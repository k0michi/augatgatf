#pragma once

#include <variant>

namespace web_audio {
class AudioNode;
}

namespace web_audio::detail {
struct MessageStart {
  double when;
  double offset;
  double duration;
  std::shared_ptr<AudioNode> node;
};

struct MessageStop {
  double when;
  std::shared_ptr<AudioNode> node;
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

using Message = std::variant<MessageStart, MessageStop, MessageTerminate,
                             MessageBeginRendering>;
} // namespace web_audio::detail