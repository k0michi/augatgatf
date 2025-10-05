#pragma once

#include <cstdint>
#include <memory>
#include <variant>

namespace web_audio {
class AudioNode;
class AudioParam;
} // namespace web_audio

namespace web_audio::details {
struct AudioNodeOutput {
  std::uint32_t sourceIndex;
  std::variant<std::weak_ptr<AudioNode>, std::weak_ptr<AudioParam>> destination;
  std::uint32_t destinationIndex;
};
} // namespace web_audio::details