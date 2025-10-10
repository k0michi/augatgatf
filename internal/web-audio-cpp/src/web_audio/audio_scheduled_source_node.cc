#include "web_audio/audio_scheduled_source_node.hh"

#include "web_audio/base_audio_context.hh"

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
  context->queueMessage(detail::MessageAudioScheduledSourceNodeStart{
      when, 0, 0,
      std::dynamic_pointer_cast<AudioScheduledSourceNode>(shared_from_this())});

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
  context->queueMessage(detail::MessageAudioScheduledSourceNodeStop{
      when,
      std::dynamic_pointer_cast<AudioScheduledSourceNode>(shared_from_this())});
}

bool AudioScheduledSourceNode::isPlaying() const {
  auto currentTime = context_.lock()->getCurrentTime();
  return sourceStarted_ && currentTime >= startTime_ && currentTime < stopTime_;
}
} // namespace web_audio