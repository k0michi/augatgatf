#pragma once

#include <cstdint>
#include <memory>
#include <variant>

namespace web_audio {
class AudioNode;
class AudioParam;
} // namespace web_audio

namespace web_audio::details {

struct AudioNodeInput {
  std::variant<std::weak_ptr<AudioNode>, std::weak_ptr<AudioParam>> source;
  std::uint32_t sourceIndex;
  std::uint32_t destinationIndex;
};
} // namespace web_audio::details