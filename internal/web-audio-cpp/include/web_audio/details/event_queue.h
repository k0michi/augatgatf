#pragma once

#include <functional>
#include <mutex>
#include <queue>

namespace web_audio::details {
class EventQueue {
public:
  void push(std::function<void()> func);
  void poll();

  void swapQueue(std::queue<std::function<void()>> &out) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.swap(out);
  }

private:
  std::queue<std::function<void()>> queue_;
  std::mutex mutex_;
};
} // namespace web_audio::details

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio::details {
void EventQueue::push(std::function<void()> func) {
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(func);
}

void EventQueue::poll() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!queue_.empty()) {
    auto func = queue_.front();
    queue_.pop();
    func();
  }
}
} // namespace web_audio::details
#endif // WEB_AUDIO_IMPLEMENTATION