#include "web_audio/detail/event_queue.hh"

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