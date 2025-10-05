#pragma once

#include <variant>

namespace web_audio {
class AudioNode;
}

namespace web_audio::details {
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
// TODO: other messages

using Message = std::variant<MessageStart, MessageStop>;
} // namespace web_audio::details