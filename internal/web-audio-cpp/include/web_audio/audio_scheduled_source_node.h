#pragma once

#include "audio_node.h"
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

private:
  EventHandler *onended = nullptr;
  // [[source started]]
  bool sourceStarted_ = false;
};
} // namespace web_audio

#ifdef WEB_AUDIO_IMPLEMENTATION
#include "base_audio_context.h"

namespace web_audio {
EventHandler *AudioScheduledSourceNode ::getOnended() { return this->onended; }

void AudioScheduledSourceNode::setOnended(EventHandler *onended) {
  this->onended = onended;
}

void AudioScheduledSourceNode::start(double when) {
  // SPEC: If this AudioScheduledSourceNode internal slot [[source started]] is
  // true, an InvalidStateError exception MUST be thrown.
  if (sourceStarted_) {
    throw DOMException(
        "AudioScheduledSourceNode: start has already been called",
        "InvalidStateError");
  }

  // SPEC: Check for any errors that must be thrown due to parameter constraints
  // described below. If any exception is thrown during this step, abort those
  // steps.
  if (when < 0) {
    throw DOMException(
        "AudioScheduledSourceNode: start parameter 'when' must be non-negative",
        "RangeError");
  }

  // SPEC: Set the internal slot [[source started]] on this
  // AudioScheduledSourceNode to true.
  sourceStarted_ = true;

  // SPEC: Queue a control message to start the AudioScheduledSourceNode,
  // including the parameter values in the message.
  auto context = context_.lock();
  context->queueMessage(details::MessageStart{when, 0, 0, shared_from_this()});

  // SPEC: Send a control message to the associated AudioContext to start
  // running its rendering thread only when all the following conditions are
  // met:
}

void AudioScheduledSourceNode::stop(double when) {
  // SPEC: If this AudioScheduledSourceNode internal slot [[source started]] is
  // not true, an InvalidStateError exception MUST be thrown.
  if (!sourceStarted_) {
    throw DOMException(
        "AudioScheduledSourceNode: start must be called before stop",
        "InvalidStateError");
  }

  // SPEC: Check for any errors that must be thrown due to parameter constraints
  // described below.
  if (when < 0) {
    throw DOMException(
        "AudioScheduledSourceNode: stop parameter 'when' must be non-negative",
        "RangeError");
  }

  // SPEC: Queue a control message to stop the AudioScheduledSourceNode,
  // including the parameter values in the message.
  auto context = context_.lock();
  context->queueMessage(details::MessageStop{when, shared_from_this()});
}
} // namespace web_audio
#endif