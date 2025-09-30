#pragma once

#include <vector>

#include "details/event_queue.h"

namespace web_audio {
class PromiseBase {
public:
  PromiseBase() = default;
  virtual ~PromiseBase() noexcept = default;
};

enum class PromiseState {
  ePending,
  eFulfilled,
  eRejected,
};

template <typename T> class PromiseInternal {
public:
  PromiseInternal(details::EventQueue &queue) : eventQueue_(queue) {}

  void resolve(T &&value) {
    state_ = PromiseState::eFulfilled;
    value_ = std::move(value);

    for (const auto &callback : fulfillCallbacks_) {
      eventQueue_.push([callback, value = value_]() { callback(value); });
    }

    fulfillCallbacks_.clear();
    rejectCallbacks_.clear();
  }

  void reject(std::exception_ptr exception) {
    state_ = PromiseState::eRejected;
    exception_ = exception;

    for (const auto &callback : rejectCallbacks_) {
      eventQueue_.push(
          [callback, exception = exception_]() { callback(exception); });
    }

    fulfillCallbacks_.clear();
    rejectCallbacks_.clear();
  }

  void then(std::function<void(T)> onFulfilled) {
    if (state_ == PromiseState::eFulfilled) {
      onFulfilled(value_);
      return;
    }

    if (state_ == PromiseState::eRejected) {
      return;
    }

    fulfillCallbacks_.push_back(onFulfilled);
  }

  void catch_(std::function<void(std::exception_ptr)> onRejected) {
    if (state_ == PromiseState::eRejected) {
      onRejected(exception_);
      return;
    }

    if (state_ == PromiseState::eFulfilled) {
      return;
    }

    rejectCallbacks_.push_back(onRejected);
  }

private:
  std::vector<std::function<void(T)>> fulfillCallbacks_;
  std::vector<std::function<void(std::exception_ptr)>> rejectCallbacks_;
  details::EventQueue &eventQueue_;
  T value_;
  std::exception_ptr exception_;
  PromiseState state_ = PromiseState::ePending;
};
} // namespace web_audio

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio {
template <typename T> class Promise : public PromiseBase {
public:
  Promise(details::EventQueue &queue) {
    internal_ = std::make_shared<PromiseInternal<T>>(queue);
  }

  // Move constructor
  Promise(Promise &&other) noexcept : internal_(std::move(other.internal_)) {}

  // Move assignment operator
  Promise &operator=(Promise &&other) noexcept {
    if (this != &other) {
      internal_ = std::move(other.internal_);
    }
    return *this;
  }

  void then(std::function<void(T)> onFulfilled) {
    internal_->then(onFulfilled);
  }

  void catch_(std::function<void(std::exception_ptr)> onRejected) {
    internal_->catch_(onRejected);
  }

  /**
   * Internal.
   */
  std::shared_ptr<PromiseInternal<T>> getInternal() const { return internal_; }

private:
  std::shared_ptr<PromiseInternal<T>> internal_;
};
} // namespace web_audio
#endif // WEB_AUDIO_IMPLEMENTATION