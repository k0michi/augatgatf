#pragma once

#include <functional>
#include <mutex>
#include <queue>

#include "common.hh"

namespace web_audio::details {
class EventQueue {
public:
  void push(std::function<void()> func);
  void poll();

  void swapQueue(std::queue<std::function<void()>> &out) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.swap(out);
  }

  WEB_AUDIO_PRIVATE : std::queue<std::function<void()>> queue_;
  std::mutex mutex_;
};
} // namespace web_audio::details