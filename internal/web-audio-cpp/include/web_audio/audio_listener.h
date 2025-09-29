#pragma once

#include "audio_param.h"

namespace web_audio {
class AudioListener {
private:
  AudioListener() = default;
  ~AudioListener() = default;

public:
  AudioParam &getPositionX();
  const AudioParam &getPositionX() const;
  AudioParam &getPositionY();
  const AudioParam &getPositionY() const;
  AudioParam &getPositionZ();
  const AudioParam &getPositionZ() const;
  AudioParam &getForwardX();
  const AudioParam &getForwardX() const;
  AudioParam &getForwardY();
  const AudioParam &getForwardY() const;
  AudioParam &getForwardZ();
  const AudioParam &getForwardZ() const;
  AudioParam &getUpX();
  const AudioParam &getUpX() const;
  AudioParam &getUpY();
  const AudioParam &getUpY() const;
  AudioParam &getUpZ();
  const AudioParam &getUpZ() const;

private:
  AudioParam positionX;
  AudioParam positionY;
  AudioParam positionZ;
  AudioParam forwardX;
  AudioParam forwardY;
  AudioParam forwardZ;
  AudioParam upX;
  AudioParam upY;
  AudioParam upZ;
};
} // namespace web_audio

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio {
AudioParam &AudioListener::getPositionX() { return positionX; }

const AudioParam &AudioListener::getPositionX() const { return positionX; }

AudioParam &AudioListener::getPositionY() { return positionY; }

const AudioParam &AudioListener::getPositionY() const { return positionY; }

AudioParam &AudioListener::getPositionZ() { return positionZ; }

const AudioParam &AudioListener::getPositionZ() const { return positionZ; }

AudioParam &AudioListener::getForwardX() { return forwardX; }

const AudioParam &AudioListener::getForwardX() const { return forwardX; }

AudioParam &AudioListener::getForwardY() { return forwardY; }

const AudioParam &AudioListener::getForwardY() const { return forwardY; }

AudioParam &AudioListener::getForwardZ() { return forwardZ; }

const AudioParam &AudioListener::getForwardZ() const { return forwardZ; }

AudioParam &AudioListener::getUpX() { return upX; }

const AudioParam &AudioListener::getUpX() const { return upX; }

AudioParam &AudioListener::getUpY() { return upY; }

const AudioParam &AudioListener::getUpY() const { return upY; }

AudioParam &AudioListener::getUpZ() { return upZ; }

const AudioParam &AudioListener::getUpZ() const { return upZ; }
} // namespace web_audio
#endif