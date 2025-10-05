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

  bool operator==(const AudioNodeInput &other) const {
    if (sourceIndex != other.sourceIndex ||
        destinationIndex != other.destinationIndex) {
      return false;
    }

    auto cmp = [](const auto &a, const auto &b) -> bool {
      return !a.owner_before(b) && !b.owner_before(a);
    };

    if (source.index() != other.source.index()) {
      return false;
    }

    if (source.index() == 0) {
      return cmp(std::get<0>(source), std::get<0>(other.source));
    } else {
      return cmp(std::get<1>(source), std::get<1>(other.source));
    }
  }

  bool operator!=(const AudioNodeInput &other) const {
    return !(*this == other);
  }
};
} // namespace web_audio::details