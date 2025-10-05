#include "web_audio/detail/message_queue.hh"

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