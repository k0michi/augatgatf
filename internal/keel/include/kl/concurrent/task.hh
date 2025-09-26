#ifndef KL_CONCURRENT_TASK_HH
#define KL_CONCURRENT_TASK_HH

#include <concepts>
#include <coroutine>
#include <memory>
#include <optional>

namespace kl::concurrent {
template <typename T> struct TaskInternal {
  T mValue;
  bool mReady = false;
  std::optional<std::coroutine_handle<>> mAwaiter;
};

template <> struct TaskInternal<void> {
  bool mReady = false;
  std::optional<std::coroutine_handle<>> mAwaiter;
};

template <typename T> struct Task {
  struct promise_type {
    Task get_return_object() { return Task{*this}; }

    std::suspend_never initial_suspend() noexcept { return {}; }

    std::suspend_never final_suspend() noexcept { return {}; }

    void return_value(const T &value) {
      mInternal->mValue = value;
      mInternal->mReady = true;

      if (mInternal->mAwaiter) {
        mInternal->mAwaiter->resume();
      }
    }

    void unhandled_exception() {}

  public:
    std::shared_ptr<TaskInternal<T>> mInternal =
        std::make_shared<TaskInternal<T>>();
  };

  explicit Task(promise_type &p) : mInternal(p.mInternal) {}

  bool await_ready() const noexcept { return mInternal->mReady; }

  void await_suspend(std::coroutine_handle<> handle) {
    mInternal->mAwaiter = handle;
  }

  T await_resume() const { return mInternal->mValue; }

private:
  std::shared_ptr<TaskInternal<T>> mInternal;
};

template <> struct Task<void> {
  struct promise_type {
    Task get_return_object() { return Task{*this}; }

    std::suspend_never initial_suspend() noexcept { return {}; }

    std::suspend_never final_suspend() noexcept { return {}; }

    void return_void() {
      mInternal->mReady = true;

      if (mInternal->mAwaiter) {
        mInternal->mAwaiter->resume();
      }
    }

    void unhandled_exception() {}

  public:
    std::shared_ptr<TaskInternal<void>> mInternal =
        std::make_shared<TaskInternal<void>>();
  };

  explicit Task(promise_type &p) : mInternal(p.mInternal) {}

  bool await_ready() const noexcept { return mInternal->mReady; }

  void await_suspend(std::coroutine_handle<> handle) {
    mInternal->mAwaiter = handle;
  }

  void await_resume() const {}

private:
  std::shared_ptr<TaskInternal<void>> mInternal;
};
} // namespace kl::concurrent
#endif // KL_CONCURRENT_TASK_HH
