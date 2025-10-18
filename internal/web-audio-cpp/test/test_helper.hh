#pragma once

#include <web_audio.hh>

namespace {
class TestHelper {
public:
  static std::shared_ptr<web_audio::OfflineAudioContext>
  createOfflineContext() {
    return web_audio::OfflineAudioContext::create(2, 128, 44100.0f);
  }

  static std::shared_ptr<web_audio::AudioBuffer>
  renderOffline(std::shared_ptr<web_audio::OfflineAudioContext> context) {
    auto promise = context->startRendering();

    bool called = false;
    std::shared_ptr<web_audio::AudioBuffer> result;
    std::exception_ptr exceptionPtr;

    promise.then([&](std::shared_ptr<web_audio::AudioBuffer> buffer) {
      called = true;
      result = buffer;
    });
    promise.catch_([&](std::exception_ptr exception) {
      called = true;
      exceptionPtr = exception;
    });

    while (!called) {
      context->processEvents();
    }

    if (exceptionPtr) {
      std::rethrow_exception(exceptionPtr);
    }

    return result;
  }
};
} // namespace