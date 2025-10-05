#pragma once

#include <memory>

namespace web_audio {
class AudioParam;
class BaseAudioContext;

class AudioListener : public std::enable_shared_from_this<AudioListener> {
private:
  AudioListener() = default;

public:
  ~AudioListener() = default;

  static std::shared_ptr<AudioListener>
  create(std::shared_ptr<BaseAudioContext> context);

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

  /**
   * Returns the context this AudioListener belongs to. Throws if the context
   * has expired.
   */
  std::shared_ptr<BaseAudioContext> getContext() const;

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

  std::weak_ptr<BaseAudioContext> context_;

  friend class BaseAudioContext;
  friend class AudioContext;
  friend class OfflineAudioContext;
};
} // namespace web_audio