#pragma once

#include <memory>

namespace web_audio {
class AudioParam;

class AudioListener {
private:
  AudioListener() = default;

public:
  ~AudioListener() = default;

public:
  std::shared_ptr<AudioParam> &getPositionX();
  const std::shared_ptr<AudioParam> &getPositionX() const;
  std::shared_ptr<AudioParam> &getPositionY();
  const std::shared_ptr<AudioParam> &getPositionY() const;
  std::shared_ptr<AudioParam> &getPositionZ();
  const std::shared_ptr<AudioParam> &getPositionZ() const;
  std::shared_ptr<AudioParam> &getForwardX();
  const std::shared_ptr<AudioParam> &getForwardX() const;
  std::shared_ptr<AudioParam> &getForwardY();
  const std::shared_ptr<AudioParam> &getForwardY() const;
  std::shared_ptr<AudioParam> &getForwardZ();
  const std::shared_ptr<AudioParam> &getForwardZ() const;
  std::shared_ptr<AudioParam> &getUpX();
  const std::shared_ptr<AudioParam> &getUpX() const;
  std::shared_ptr<AudioParam> &getUpY();
  const std::shared_ptr<AudioParam> &getUpY() const;
  std::shared_ptr<AudioParam> &getUpZ();
  const std::shared_ptr<AudioParam> &getUpZ() const;

private:
  std::shared_ptr<AudioParam> positionX;
  std::shared_ptr<AudioParam> positionY;
  std::shared_ptr<AudioParam> positionZ;
  std::shared_ptr<AudioParam> forwardX;
  std::shared_ptr<AudioParam> forwardY;
  std::shared_ptr<AudioParam> forwardZ;
  std::shared_ptr<AudioParam> upX;
  std::shared_ptr<AudioParam> upY;
  std::shared_ptr<AudioParam> upZ;

  friend class BaseAudioContext;
  friend class AudioContext;
  friend class OfflineAudioContext;
};
} // namespace web_audio

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio {
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
} // namespace web_audio
#endif