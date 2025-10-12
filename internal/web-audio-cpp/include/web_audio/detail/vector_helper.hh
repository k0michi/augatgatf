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
  static T getOrDefault(const std::vector<T> &vector, std::size_t index,
                        const T &defaultValue) {
    if (index >= vector.size()) {
      return defaultValue;
    }

    return vector[index];
  }
};
} // namespace web_audio::detail