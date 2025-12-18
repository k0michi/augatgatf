#ifndef KL_MATH_MATH_HH
#define KL_MATH_MATH_HH

#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>
#include <numbers>
#include <stdexcept>

namespace kl::math {
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

  template <std::floating_point T> static T step(T edge, T x) {
    return x < edge ? static_cast<T>(0) : static_cast<T>(1);
  }

  template <std::floating_point T> static T smoothstep(T edge0, T edge1, T x) {
    T t = clamp((x - edge0) / (edge1 - edge0), static_cast<T>(0),
                static_cast<T>(1));

    return t * t * (static_cast<T>(3) - static_cast<T>(2) * t);
  }

  /**
   * @see https://doc.rust-lang.org/std/primitive.i32.html#method.overflowing_add
   * @tparam T 
   * @param a 
   * @param b 
   * @return std::pair<T, bool> 
   */
  template <std::integral T>
  static std::pair<T, bool> overflowingAdd(T a, T b) {
    if constexpr (std::is_unsigned_v<T>) {
      T result = a + b;
      bool overflow = result < a;
      return {result, overflow};
    } else {
      using U = std::make_unsigned_t<T>;
      U ua = static_cast<U>(a);
      U ub = static_cast<U>(b);
      U uresult = ua + ub;
      T result = static_cast<T>(uresult);
      bool overflow = ((b > 0) && (result < a)) || ((b < 0) && (result > a));
      return {result, overflow};
    }
  }

  /**
   * @see https://doc.rust-lang.org/std/primitive.i32.html#method.checked_add
   * @tparam T 
   * @param a 
   * @param b 
   * @return std::optional<T> 
   */
  template <std::integral T> static std::optional<T> checkedAdd(T a, T b) {
    auto [result, overflow] = overflowingAdd<T>(a, b);

    if (overflow) {
      return std::nullopt;
    }

    return result;
  }

  /**
   * @see https://doc.rust-lang.org/std/primitive.i32.html#method.wrapping_add
   * @tparam T 
   * @param a 
   * @param b 
   * @return T 
   */
  template <std::integral T> static T wrappingAdd(T a, T b) {
    return static_cast<T>(static_cast<std::make_unsigned_t<T>>(a) +
                          static_cast<std::make_unsigned_t<T>>(b));
  }

  /**
   * @see https://doc.rust-lang.org/std/primitive.i32.html#method.saturating_add
   * @tparam T 
   * @param a 
   * @param b 
   * @return T 
   */
  template <std::integral T> static T saturatingAdd(T a, T b) {
    auto [result, overflow] = overflowingAdd<T>(a, b);

    if (overflow) {
      if (b > 0) {
        return std::numeric_limits<T>::max();
      } else {
        return std::numeric_limits<T>::min();
      }
    }

    return result;
  }
};
} // namespace kl::math
#endif