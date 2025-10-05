#pragma once

#include "audio_node.h"
#include "details/common.h"
#include "event_handler.h"

namespace web_audio {
class AudioScheduledSourceNode : public AudioNode {
protected:
  AudioScheduledSourceNode() = default;

public:
  // attribute EventHandler onended;
  EventHandler *getOnended();

  void setOnended(EventHandler *onended);

  void start(double when = 0);

  void stop(double when = 0);

  WEB_AUDIO_PRIVATE : EventHandler *onended = nullptr;
  // [[source started]]
  bool sourceStarted_ = false;
};
} // namespace web_audio