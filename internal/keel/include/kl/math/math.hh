#ifndef KL_MATH_MATH_HH
#define KL_MATH_MATH_HH

#include <cmath>
#include <concepts>
#include <limits>
#include <numbers>

class Math {
public:
  template <std::floating_point T> static T toRadians(T degrees) {
    return std::numbers::pi_v<T> * degrees / static_cast<T>(180);
  }

  template <std::floating_point T> static T toDegrees(T radians) {
    return radians * static_cast<T>(180) / std::numbers::pi_v<T>;
  }
};
#endif