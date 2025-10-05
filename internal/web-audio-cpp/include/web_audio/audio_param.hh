#pragma once

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <limits>
#include <memory>
#include <set>
#include <stdexcept>
#include <variant>
#include <vector>

#include "automation_rate.hh"
#include "details/audio_node_input.hh"
#include "details/common.hh"
#include "details/param_event.hh"

namespace web_audio {
class BaseAudioContext;
class AudioNode;
class AudioListener;

namespace details {
class AudioGraph;
}

class AudioParam : public std::enable_shared_from_this<AudioParam> {
  WEB_AUDIO_PRIVATE : AudioParam() = default;

  static std::shared_ptr<AudioParam>
  create(std::variant<std::weak_ptr<AudioNode>, std::weak_ptr<AudioListener>>
             owner,
         float defaultValue = 0.0f,
         float minValue = -std::numeric_limits<float>::infinity(),
         float maxValue = std::numeric_limits<float>::infinity(),
         AutomationRate automationRate = AutomationRate::eARate,
         bool allowARate = true);

public:
  virtual ~AudioParam() noexcept = default;

  // SPEC: attribute float value;
  float getValue() const;

  void setValue(float value);

  // SPEC: attribute AutomationRate automationRate;
  AutomationRate getAutomationRate() const;

  void setAutomationRate(AutomationRate automationRate);

  // SPEC: readonly attribute float defaultValue;
  float getDefaultValue() const;

  // SPEC: readonly attribute float minValue;
  float getMinValue() const;

  // SPEC: readonly attribute float maxValue;
  float getMaxValue() const;

  // SPEC: AudioParam setValueAtTime (float value, double startTime);
  std::shared_ptr<AudioParam> setValueAtTime(float value, double startTime);

  // SPEC: AudioParam linearRampToValueAtTime (float value, double endTime);
  std::shared_ptr<AudioParam> linearRampToValueAtTime(float value,
                                                      double endTime);

  // SPEC: AudioParam exponentialRampToValueAtTime (float value, double
  // endTime);
  std::shared_ptr<AudioParam> exponentialRampToValueAtTime(float value,
                                                           double endTime);

  // SPEC: AudioParam setTargetAtTime (float target, double startTime, float
  // timeConstant);
  std::shared_ptr<AudioParam> setTargetAtTime(float target, double startTime,
                                              float timeConstant);

  // SPEC: AudioParam setValueCurveAtTime (sequence<float> values, double
  // startTime, double duration);
  std::shared_ptr<AudioParam>
  setValueCurveAtTime(const std::vector<float> &values, double startTime,
                      double duration);

  // SPEC: AudioParam cancelScheduledValues (double cancelTime);
  std::shared_ptr<AudioParam> cancelScheduledValues(double cancelTime);

  // SPEC: AudioParam cancelAndHoldAtTime (double cancelTime);
  std::shared_ptr<AudioParam> cancelAndHoldAtTime(double cancelTime);

  /**
   * Returns the last event whose time is less than or equal to `time`.
   */
  std::set<details::ParamEvent, details::ParamEventLess>::iterator
  floorEvent(double time);

  /**
   * Returns the first event whose time is greater than `time`.
   */
  std::set<details::ParamEvent, details::ParamEventLess>::iterator
  higherEvent(double time);

  /**
   * Throws NotSupportedError if time is included in a value curve event.
   */
  void checkValueCurve(double time) const;

  /**
   * Get the value at the end of the given event.
   */
  float getEndValue(std::set<details::ParamEvent,
                             details::ParamEventLess>::iterator event) const;

  /**
   * Get the value at the beginning of the given event.
   */
  float getStartValue(std::set<details::ParamEvent,
                               details::ParamEventLess>::iterator event) const;

  void computeIntrinsicValues(double startTime, std::vector<float> &outputs);

  /**
   * Returns the owner of this AudioParam (either an AudioNode or an
   * AudioListener). Throws if the owner has expired.
   */
  std::variant<std::shared_ptr<AudioNode>, std::shared_ptr<AudioListener>>
  getOwner() const;

  /**
   * Returns the context this AudioParam belongs to. Throws if the context
   * has expired.
   */
  std::shared_ptr<BaseAudioContext> getContext() const;

  WEB_AUDIO_PRIVATE :
      // [[current value]]
      float currentValue_;
  AutomationRate automationRate_;
  float defaultValue_;
  float minValue_;
  float maxValue_;
  bool allowARate_;
  std::set<details::ParamEvent, details::ParamEventLess> events_;
  std::uint32_t eventIndex_ = 0;
  std::variant<std::weak_ptr<AudioNode>, std::weak_ptr<AudioListener>> owner_;
  std::vector<details::AudioNodeInput> inputs_;

  friend class DelayNode;
  friend class details::AudioGraph;
  friend class AudioNode;
};
} // namespace web_audio