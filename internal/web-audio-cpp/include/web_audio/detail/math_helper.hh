#pragma once

namespace web_audio::detail {
class MathHelper {
public:
  template <typename T> static T nextPowerOfTwo(T value) {
    if (value <= 0) {
      return 1;
    }

    T power = 1;

    while (power < value) {
      power <<= 1;
    }

    return power;
  }
};
} // namespace web_audio::detail