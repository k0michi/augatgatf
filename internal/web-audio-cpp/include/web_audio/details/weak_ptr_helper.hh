#pragma once

#include <compare>
#include <memory>

namespace web_audio::details {
class WeakPtrHelper {
public:
  template <typename T>
  static std::strong_ordering compare(const std::weak_ptr<T> &a,
                                      const std::weak_ptr<T> &b) {
    if (a.owner_before(b)) {
      return std::strong_ordering::less;
    }

    if (b.owner_before(a)) {
      return std::strong_ordering::greater;
    }

    return std::strong_ordering::equal;
  }

  template <typename T>
  static std::strong_ordering compare(const std::shared_ptr<T> &a,
                                      const std::weak_ptr<T> &b) {
    if (a.owner_before(b)) {
      return std::strong_ordering::less;
    }

    if (b.owner_before(a)) {
      return std::strong_ordering::greater;
    }

    return std::strong_ordering::equal;
  }

  template <typename T>
  static std::strong_ordering compare(const std::weak_ptr<T> &a,
                                      const std::shared_ptr<T> &b) {
    return compare(b, a);
  }
};
} // namespace web_audio::details