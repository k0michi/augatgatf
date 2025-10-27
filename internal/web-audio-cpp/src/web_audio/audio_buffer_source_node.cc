#include "web_audio/audio_buffer_source_node.hh"

#include <cmath>

#include "web_audio/audio_buffer.hh"
#include "web_audio/audio_param.hh"
#include "web_audio/base_audio_context.hh"
#include "web_audio/dom_exception.hh"

namespace web_audio {
std::shared_ptr<AudioBufferSourceNode>
AudioBufferSourceNode::create(std::shared_ptr<BaseAudioContext> context,
                              AudioBufferSourceOptions options) {
  auto node =
      std::shared_ptr<AudioBufferSourceNode>(new AudioBufferSourceNode());
  node->initialize(context);

  node->numberOfInputs_ = 0;
  node->numberOfOutputs_ = 1;
  node->setChannelCount(options.channelCount.value_or(2));
  node->setChannelCountMode(
      options.channelCountMode.value_or(ChannelCountMode::eMax));
  node->setChannelInterpretation(
      options.channelInterpretation.value_or(ChannelInterpretation::eSpeakers));

  node->playbackRate_ = AudioParam::create(
      node, 1.0f, std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::max(), AutomationRate::eKRate, false);
  node->playbackRate_->setValue(options.playbackRate);
  node->detune_ = AudioParam::create(
      node, 0.0f, std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::max(), AutomationRate::eKRate, false);
  node->detune_->setValue(options.detune);

  node->setBuffer(options.buffer);
  node->setLoop(options.loop);
  node->setLoopStart(options.loopStart);
  node->setLoopEnd(options.loopEnd);

  node->dt_ = 1.0 / context->getSampleRate();

  return node;
}

void AudioBufferSourceNode::setBuffer(std::shared_ptr<AudioBuffer> buffer) {
  // SPEC: Let new buffer be the AudioBuffer or null value to be assigned to
  // buffer.

  // SPEC: If new buffer is not null and [[buffer set]] is true, throw an
  // InvalidStateError and abort these steps.
  if (buffer_) {
    throw DOMException{"AudioBufferSourceNode buffer is already set.",
                       "InvalidStateError"};
  }

  // SPEC: If new buffer is not null, set [[buffer set]] to true.

  // SPEC: Assign new buffer to the buffer attribute.
  buffer_ = buffer;

  // SPEC: If start() has previously been called on this node, perform the
  // operation acquire the content on buffer.
  if (buffer_) {
    bufferClone_ = buffer_->copy();
  } else {
    bufferClone_ = nullptr;
  }
}

std::shared_ptr<AudioBuffer> AudioBufferSourceNode::getBuffer() const {
  return buffer_;
}

std::shared_ptr<AudioParam> AudioBufferSourceNode::getPlaybackRate() const {
  return playbackRate_;
}

std::shared_ptr<AudioParam> AudioBufferSourceNode::getDetune() const {
  return detune_;
}

void AudioBufferSourceNode::setLoop(bool loop) { loop_ = loop; }

bool AudioBufferSourceNode::getLoop() const { return loop_; }

double AudioBufferSourceNode::getLoopStart() const { return loopStart_; }

void AudioBufferSourceNode::setLoopStart(double loopStart) {
  loopStart_ = loopStart;
}

double AudioBufferSourceNode::getLoopEnd() const { return loopEnd_; }

void AudioBufferSourceNode::setLoopEnd(double loopEnd) { loopEnd_ = loopEnd; }

void AudioBufferSourceNode::start(double when, double offset) {
  start(when, offset, std::numeric_limits<double>::infinity());
}

void AudioBufferSourceNode::start(double when, double offset, double duration) {
  // SPEC: A RangeError exception MUST be thrown if when is negative.
  if (when < 0) {
    throw DOMException(
        "AudioBufferSourceNode: playbackRate must be a positive value",
        "RangeError");
  }

  // SPEC: A RangeError exception MUST be thrown if offset is negative.
  if (offset < 0) {
    throw DOMException("AudioBufferSourceNode: offset must be a positive value",
                       "RangeError");
  }

  // SPEC: A RangeError exception MUST be thrown if duration is negative.
  if (duration < 0) {
    throw DOMException(
        "AudioBufferSourceNode: duration must be a positive value",
        "RangeError");
  }

  // TODO: Dispatch to rendering thread
  start_ = when;
  offset_ = offset;
  duration_ = duration;

  bufferClone_ = buffer_->copy();
}

float AudioBufferSourceNode::playbackSignal(std::uint32_t channel,
                                            double time) const {
  // Linear interpolation
  if (!bufferClone_) {
    return 0.0f;
  }

  double sampleRate = bufferClone_->getSampleRate();

  double bufferPos = time * sampleRate;
  std::uint32_t index1 = static_cast<std::uint32_t>(std::floor(bufferPos));
  std::uint32_t index2 = index1 + 1;
  float fraction = static_cast<float>(bufferPos - index1);
  const auto &channelData = bufferClone_->getChannelData(channel);
  float sample1 =
      (index1 < bufferClone_->getLength()) ? channelData[index1] : 0.0f;
  float sample2 =
      (index2 < bufferClone_->getLength()) ? channelData[index2] : 0.0f;
  return sample1 + (sample2 - sample1) * fraction;
}

void AudioBufferSourceNode::process(
    const std::vector<detail::RenderQuantum> &inputs,
    std::vector<detail::RenderQuantum> &outputs,
    const detail::ParamCollection &params) {
  //   if (!isPlaying()) {
  //     return;
  //   }

  auto &output = outputs[0];
  outputs[0] = detail::RenderQuantum(bufferClone_->getNumberOfChannels(),
                                     output.getLength());

  auto context = getContext();

  auto currentTime = context->getCurrentTime();

  auto computedPlaybackRate =
      params.getValue(playbackRate_, 0) *
      std::pow(2, params.getValue(detune_, 0) / 1200.0f);

  double actualLoopStart, actualLoopEnd;
  if (loop_ && bufferClone_) {
    if (loopStart_ >= 0 && loopEnd_ > 0 && loopStart_ < loopEnd_) {
      actualLoopStart = loopStart_;
      actualLoopEnd = std::min(loopEnd_, bufferClone_->getDuration());
    } else {
      actualLoopStart = 0.0;
      actualLoopEnd = bufferClone_->getDuration();
    }
  } else {
    enteredLoop_ = false;
  }

  if (!bufferClone_) {
    stop_ = currentTime;
  }

  for (std::uint32_t index = 0; index < output.getLength(); ++index) {
    if (currentTime < start_ || currentTime >= stop_ ||
        bufferTimeElapsed_ >= duration_) {
      for (std::uint32_t ch = 0; ch < output.getNumberOfChannels(); ++ch) {
        output[ch][index] = 0.0f;
      }

      currentTime += dt_;
      continue;
    }

    if (!started_) {
      if (loop_ && computedPlaybackRate >= 0 && offset_ >= actualLoopEnd) {
        offset_ = actualLoopEnd;
      }

      if (loop_ && computedPlaybackRate < 0 && offset_ <= actualLoopStart) {
        offset_ = actualLoopStart;
      }

      bufferTime_ = offset_;
      started_ = true;
    }

    if (loop_) {
      if (!enteredLoop_) {
        if (offset_ < actualLoopEnd && bufferTime_ >= actualLoopStart) {
          enteredLoop_ = true;
        }

        if (offset_ >= actualLoopEnd && bufferTime_ < actualLoopEnd) {
          enteredLoop_ = true;
        }
      }

      if (enteredLoop_) {
        while (bufferTime_ >= actualLoopEnd) {
          bufferTime_ -= actualLoopEnd - actualLoopStart;
        }

        while (bufferTime_ < actualLoopStart) {
          bufferTime_ += actualLoopEnd - actualLoopStart;
        }
      }
    }

    if (bufferTime_ >= 0 && bufferTime_ < bufferClone_->getDuration()) {
      for (std::uint32_t ch = 0; ch < output.getNumberOfChannels(); ++ch) {
        output[ch][index] = playbackSignal(ch, bufferTime_);
      }
    } else {
      for (std::uint32_t ch = 0; ch < output.getNumberOfChannels(); ++ch) {
        output[ch][index] = 0.0f;
      }
    }

    bufferTime_ += dt_ * computedPlaybackRate;
    bufferTimeElapsed_ += dt_;
    currentTime += dt_;
  }

  if (currentTime >= stop_) {
    // TODO:
  }
}

std ::vector<std::shared_ptr<AudioParam>>
AudioBufferSourceNode::getParams() const {
  return {playbackRate_, detune_};
}
} // namespace web_audio