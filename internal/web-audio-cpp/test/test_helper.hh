#pragma once

#include <web_audio.hh>

namespace {
class TestHelper {
public:
  static std::shared_ptr<web_audio::OfflineAudioContext>
  createOfflineContext() {
    return web_audio::OfflineAudioContext::create(2, 128, 44100.0f);
  }
};
} // namespace