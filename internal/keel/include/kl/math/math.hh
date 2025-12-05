#ifndef KL_MATH_MATH_HH
#define KL_MATH_MATH_HH

#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>
#include <numbers>
#include <stdexcept>

class Math {
public:
  template <std::floating_point T> static T toRadians(T degrees) {
    return std::numbers::pi_v<T> * degrees / static_cast<T>(180);
  }

  template <std::floating_point T> static T toDegrees(T radians) {
    return radians * static_cast<T>(180) / std::numbers::pi_v<T>;
  }

  // REF:
  // https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html#clamp(long,int,int)
  template <std::integral T> static T clamp(T value, T min, T max) {
    if (min > max) {
      throw std::invalid_argument("Illegal clamp arguments");
    }

    if (value < min) {
      return min;
    }

    if (value > max) {
      return max;
    }

    return value;
  }

  // REF:
  // https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html#clamp(double,double,double)
  template <std::floating_point T> static T clamp(T value, T min, T max) {
    if (std::isnan(min) || std::isnan(max) || min > max ||
        (min == 0.0 && std::signbit(min) == 0 && max == 0.0 &&
         std::signbit(max) != 0)) {
      throw std::invalid_argument("Illegal clamp arguments");
    }

    if (std::isnan(value)) {
      return value;
    }

    if (value < min) {
      return min;
    }

    if (value > max) {
      return max;
    }

    return value;
  }

  template <std::floating_point T> static T mix(T x, T y, T a) {
    return x * (static_cast<T>(1) - a) + y * a;
  }

  template <std::floating_point T> static T mix(T x, T y, bool a) {
    return a ? y : x;
  }

  template <std::integral T> static T mix(T x, T y, bool a) {
    return a ? y : x;
  }

  static bool mix(bool x, bool y, bool a) { return a ? y : x; }
};
#endif