#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

#include "message.h"

namespace web_audio::details {
class MessageQueue {
private:
  std::queue<Message> queue_;
  std::mutex mtx_;
  std::condition_variable cv_;

public:
  template <typename MessageType> void push(MessageType &&value) {
    {
      std::lock_guard<std::mutex> lock(mtx_);
      queue_.emplace(std::forward<MessageType>(value));
      cv_.notify_one();
    }
  }

  Message pop();

  void swap(MessageQueue &other);
};
} // namespace web_audio::details

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio::details {
Message MessageQueue::pop() {
  std::unique_lock<std::mutex> lock(mtx_);
  cv_.wait(lock, [this] { return !queue_.empty(); });
  Message value = queue_.front();
  queue_.pop();
  return value;
}

void MessageQueue::swap(MessageQueue &other) {
  std::lock(mtx_, other.mtx_);
  std::lock_guard<std::mutex> lock1(mtx_, std::adopt_lock);
  std::lock_guard<std::mutex> lock2(other.mtx_, std::adopt_lock);
  queue_.swap(other.queue_);
}
} // namespace web_audio::details
#endif // WEB_AUDIO_IMPLEMENTATION