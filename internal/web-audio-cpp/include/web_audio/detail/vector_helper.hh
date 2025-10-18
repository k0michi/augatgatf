#pragma once

#include <vector>

namespace web_audio::detail {
class VectorHelper {
public:
  /**
   * If the size of the vector is enough, set the value at the specified index.
   * Otherwise, resize the vector and set the value.
   */
  template <typename T>
  static void resizeAndSet(std::vector<T> &vector, std::size_t index,
                           const T &value) {
    if (index >= vector.size()) {
      vector.resize(index + 1);
    }
    vector[index] = value;
  }

  template <typename T>
  static void resizeAndSet(std::vector<T> &vector, std::size_t index,
                           T &&value) {
    if (index >= vector.size()) {
      vector.resize(index + 1);
    }
    vector[index] = std::move(value);
  }

  template <typename T>
  static T getOrDefault(const std::vector<T> &vector, std::size_t index,
                        const T &defaultValue) {
    if (index >= vector.size()) {
      return defaultValue;
    }

    return vector[index];
  }

  template <typename T>
  static void shiftLeft(std::vector<T> &vec, std::size_t shift) {
    if (shift >= vec.size()) {
      std::fill(vec.begin(), vec.end(), T{});
      return;
    }
    std::move(vec.begin() + shift, vec.end(), vec.begin());
    std::fill(vec.end() - shift, vec.end(), T{});
  }
};
} // namespace web_audio::detail