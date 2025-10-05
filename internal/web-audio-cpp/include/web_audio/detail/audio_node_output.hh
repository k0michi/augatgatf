#pragma once

#include <cstdint>
#include <memory>
#include <variant>

namespace web_audio {
class AudioNode;
class AudioParam;
} // namespace web_audio

namespace web_audio::detail {
struct AudioNodeOutput {
  std::uint32_t sourceIndex;
  std::variant<std::weak_ptr<AudioNode>, std::weak_ptr<AudioParam>> destination;
  std::uint32_t destinationIndex;

  bool operator==(const AudioNodeOutput &other) const {
    if (sourceIndex != other.sourceIndex ||
        destinationIndex != other.destinationIndex) {
      return false;
    }

    auto cmp = [](const auto &a, const auto &b) -> bool {
      return !a.owner_before(b) && !b.owner_before(a);
    };

    if (destination.index() != other.destination.index()) {
      return false;
    }

    if (destination.index() == 0) {
      return cmp(std::get<0>(destination), std::get<0>(other.destination));
    } else {
      return cmp(std::get<1>(destination), std::get<1>(other.destination));
    }
  }

  bool operator!=(const AudioNodeOutput &other) const {
    return !(*this == other);
  }
};
} // namespace web_audio::detail