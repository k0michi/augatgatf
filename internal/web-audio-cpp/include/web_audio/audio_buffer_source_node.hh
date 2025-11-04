#pragma once

#include "audio_buffer_source_options.hh"
#include "audio_scheduled_source_node.hh"
#include "detail/common.hh"

namespace web_audio {
class AudioBufferSourceNode : public AudioScheduledSourceNode {
public:
  // SPEC: constructor (BaseAudioContext context, optional
  // AudioBufferSourceOptions options = {});
  static std::shared_ptr<AudioBufferSourceNode>
  create(std::shared_ptr<BaseAudioContext> context,
         AudioBufferSourceOptions options = {});

  // SPEC: attribute AudioBuffer? buffer;
  void setBuffer(std::shared_ptr<AudioBuffer> buffer);

  std::shared_ptr<AudioBuffer> getBuffer() const;

  // SPEC: readonly attribute AudioParam playbackRate;
  std::shared_ptr<AudioParam> getPlaybackRate() const;

  // SPEC: readonly attribute AudioParam detune;
  std::shared_ptr<AudioParam> getDetune() const;

  // SPEC: attribute boolean loop;
  void setLoop(bool loop);

  bool getLoop() const;

  // SPEC: attribute double loopStart;
  double getLoopStart() const;

  void setLoopStart(double loopStart);

  // SPEC: attribute double loopEnd;
  double getLoopEnd() const;

  void setLoopEnd(double loopEnd);

  // SPEC: undefined start (optional double when = 0, optional double offset,
  // optional double duration);
  void start(double when = 0, double offset = 0);

  void start(double when, double offset, double duration);

  float playbackSignal(std::uint32_t channel, double time) const;

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  std::vector<std::shared_ptr<AudioParam>> getParams() const override;

  WEB_AUDIO_PRIVATE : AudioBufferSourceNode() = default;

  std::shared_ptr<AudioBuffer> buffer_;
  std::shared_ptr<AudioBuffer> bufferClone_;
  std::shared_ptr<AudioParam> playbackRate_;
  std::shared_ptr<AudioParam> detune_;
  bool loop_ = false;
  double loopStart_ = 0.0;
  double loopEnd_ = 0.0;
  double start_ = 0.0;
  double offset_ = 0.0;
  double duration_ = std::numeric_limits<double>::infinity();
  double stop_ = std::numeric_limits<double>::infinity();
  double bufferTime_ = 0.0;
  bool started_ = false;
  bool enteredLoop_ = false;
  double bufferTimeElapsed_ = 0.0;
  double dt_;
};
} // namespace web_audio