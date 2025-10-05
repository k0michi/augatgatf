#include "web_audio/audio_listener.h"

#include <stdexcept>

namespace web_audio {
std::shared_ptr<AudioListener>
AudioListener::create(std::shared_ptr<BaseAudioContext> context) {
  auto listener = std::shared_ptr<AudioListener>(new AudioListener());

  listener->context_ = context;

  return listener;
}

std::shared_ptr<AudioParam> &AudioListener::getPositionX() { return positionX; }

const std::shared_ptr<AudioParam> &AudioListener::getPositionX() const {
  return positionX;
}

std::shared_ptr<AudioParam> &AudioListener::getPositionY() { return positionY; }

const std::shared_ptr<AudioParam> &AudioListener::getPositionY() const {
  return positionY;
}

std::shared_ptr<AudioParam> &AudioListener::getPositionZ() { return positionZ; }

const std::shared_ptr<AudioParam> &AudioListener::getPositionZ() const {
  return positionZ;
}

std::shared_ptr<AudioParam> &AudioListener::getForwardX() { return forwardX; }

const std::shared_ptr<AudioParam> &AudioListener::getForwardX() const {
  return forwardX;
}

std::shared_ptr<AudioParam> &AudioListener::getForwardY() { return forwardY; }

const std::shared_ptr<AudioParam> &AudioListener::getForwardY() const {
  return forwardY;
}

std::shared_ptr<AudioParam> &AudioListener::getForwardZ() { return forwardZ; }

const std::shared_ptr<AudioParam> &AudioListener::getForwardZ() const {
  return forwardZ;
}

std::shared_ptr<AudioParam> &AudioListener::getUpX() { return upX; }

const std::shared_ptr<AudioParam> &AudioListener::getUpX() const { return upX; }

std::shared_ptr<AudioParam> &AudioListener::getUpY() { return upY; }

const std::shared_ptr<AudioParam> &AudioListener::getUpY() const { return upY; }

std::shared_ptr<AudioParam> &AudioListener::getUpZ() { return upZ; }

const std::shared_ptr<AudioParam> &AudioListener::getUpZ() const { return upZ; }

std::shared_ptr<BaseAudioContext> AudioListener::getContext() const {
  if (auto context = context_.lock()) {
    return context;
  } else {
    throw std::runtime_error("AudioListener: context has expired");
  }
}
} // namespace web_audio