#include "web_audio/detail/message_queue.hh"

namespace web_audio::detail {
Message MessageQueue::pop() {
  std::unique_lock<std::mutex> lock(mtx_);
  cv_.wait(lock, [this] { return !queue_.empty(); });
  Message value = queue_.front();
  queue_.pop();
  return value;
}

std::optional<Message> MessageQueue::tryPop() {
  std::lock_guard<std::mutex> lock(mtx_);
  if (queue_.empty()) {
    return std::nullopt;
  }
  Message value = queue_.front();
  queue_.pop();
  return value;
}

void MessageQueue::swap(MessageQueue &other) {
  std::scoped_lock lock(mtx_, other.mtx_);
  queue_.swap(other.queue_);
}
} // namespace web_audio::detail