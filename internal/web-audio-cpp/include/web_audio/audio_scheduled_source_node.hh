#pragma once

#include "audio_node.hh"
#include "detail/common.hh"
#include "event_handler.hh"

namespace web_audio {
class BaseAudioContext;

class AudioScheduledSourceNode : public AudioNode {
protected:
  AudioScheduledSourceNode() = default;

public:
  // attribute EventHandler onended;
  EventHandler *getOnended();

  void setOnended(EventHandler *onended);

  void start(double when = 0);

  void stop(double when = 0);

  /**
   * Returns true if the current time is between the scheduled start time
   * and stop time.
   */
  bool isPlaying() const;

  WEB_AUDIO_PRIVATE : EventHandler *onended = nullptr;
  // [[source started]]
  bool sourceStarted_ = false;
  double startTime_ = std::numeric_limits<double>::infinity();
  double stopTime_ = std::numeric_limits<double>::infinity();

  friend class BaseAudioContext;
};
} // namespace web_audio