#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

#include "common.h"
#include "message.h"

namespace web_audio::details {
class MessageQueue {
  WEB_AUDIO_PRIVATE : std::queue<Message> queue_;
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