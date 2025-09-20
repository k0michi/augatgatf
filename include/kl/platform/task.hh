#ifndef KL_PLATFORM_TASK_HH
#define KL_PLATFORM_TASK_HH

#include <coroutine>

namespace kl::platform {
template <typename T> struct Task {
  struct promise_type {
    T m_value;
    Task get_return_object() { return Task{this}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_value(const T &value) { m_value = value; }
    void unhandled_exception() {}
  };
  promise_type *m_promise;
  explicit Task(promise_type *promise) : m_promise(promise) {}
  T value() const { return m_promise->m_value; }
};

template <> struct Task<void> {
  struct promise_type {
    Task get_return_object() { return Task{this}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
  promise_type *m_promise;
  explicit Task(promise_type *promise) : m_promise(promise) {}
};
} // namespace kl::platform
#endif // KL_PLATFORM_TASK_HH
