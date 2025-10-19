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

  template <typename T> static T normalizeDegrees(T degrees) {
    auto result = std::fmod(degrees, static_cast<T>(360));

    if (result < 0) {
      result += static_cast<T>(360);
    }

    return result;
  }
};
} // namespace web_audio::detail