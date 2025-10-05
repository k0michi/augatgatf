#pragma once

#include <cstdint>
#include <memory>
#include <variant>

namespace web_audio {
class AudioNode;
class AudioParam;
} // namespace web_audio

namespace web_audio::detail {
struct AudioNodeInput {
  std::weak_ptr<AudioNode> source;
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

    return cmp(source, other.source);
  }

  bool operator!=(const AudioNodeInput &other) const {
    return !(*this == other);
  }
};
} // namespace web_audio::detail