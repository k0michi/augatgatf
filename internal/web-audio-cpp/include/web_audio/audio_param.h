#pragma once

#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <set>
#include <vector>

#include "automation_rate.h"
#include "details/param_event.h"

namespace web_audio {
class BaseAudioContext;

class AudioParam : public std::enable_shared_from_this<AudioParam> {
#ifdef WEB_AUDIO_TEST
public:
#else
private:
#endif
  AudioParam() = default;

  static std::shared_ptr<AudioParam>
  create(std::shared_ptr<BaseAudioContext> context = nullptr,
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

  std::set<details::ParamEvent, details::ParamEventLess>::iterator
  floorEvent(double time);

  std::set<details::ParamEvent, details::ParamEventLess>::iterator
  higherEvent(double time);

  /**
   * Get the value at the end of the given event.
   */
  float getLastValue(std::set<details::ParamEvent,
                              details::ParamEventLess>::iterator event) const;

#ifdef WEB_AUDIO_TEST
public:
#else
private:
#endif
  // [[current value]]
  float currentValue_;
  std::weak_ptr<BaseAudioContext> context_;
  AutomationRate automationRate_;
  float defaultValue_;
  float minValue_;
  float maxValue_;
  bool allowARate_;
  std::set<details::ParamEvent, details::ParamEventLess> events_;
  std::uint32_t eventIndex_ = 0;

  friend class DelayNode;
};
} // namespace web_audio

#ifdef WEB_AUDIO_IMPLEMENTATION
#include "base_audio_context.h"

namespace web_audio {
std::shared_ptr<AudioParam>
AudioParam::create(std::shared_ptr<BaseAudioContext> context,
                   float defaultValue, float minValue, float maxValue,
                   AutomationRate automationRate, bool allowARate) {
  auto instance = std::shared_ptr<AudioParam>(new AudioParam());
  instance->context_ = context;
  instance->defaultValue_ = defaultValue;
  instance->minValue_ = minValue;
  instance->maxValue_ = maxValue;
  instance->automationRate_ = automationRate;
  instance->allowARate_ = allowARate;
  return instance;
}

float AudioParam::getValue() const {
  return std::clamp(currentValue_, minValue_, maxValue_);
}

void AudioParam::setValue(float value) {
  currentValue_ = value;
  auto context = context_.lock();
  auto currentTime = context->getCurrentTime();
  setValueAtTime(value, currentTime);
}

AutomationRate AudioParam::getAutomationRate() const { return automationRate_; }

void AudioParam::setAutomationRate(AutomationRate automationRate) {
  if (automationRate == AutomationRate::eARate && !allowARate_) {
    throw DOMException("AudioParam: cannot set automationRate to 'a-rate'",
                       "InvalidStateError");
  }

  automationRate_ = automationRate;
}

float AudioParam::getDefaultValue() const { return defaultValue_; }

float AudioParam::getMinValue() const { return minValue_; }

float AudioParam::getMaxValue() const { return maxValue_; }

std::shared_ptr<AudioParam> AudioParam::setValueAtTime(float value,
                                                       double startTime) {
  if (startTime < 0 || std::isnan(startTime) || std::isinf(startTime)) {
    throw DOMException(
        "AudioParam: setValueAtTime parameter 'startTime' must be non-negative",
        "RangeError");
  }

  events_.emplace(details::ParamEventSetValue{eventIndex_, value, startTime});
  return shared_from_this();
}

std::shared_ptr<AudioParam>
AudioParam::linearRampToValueAtTime(float value, double endTime) {
  if (endTime < 0 || std::isnan(endTime) || std::isinf(endTime)) {
    throw DOMException(
        "AudioParam: linearRampToValueAtTime parameter 'endTime' must be "
        "non-negative",
        "RangeError");
  }

  events_.emplace(details::ParamEventLinearRamp{eventIndex_, value, endTime});
  return shared_from_this();
}

std::shared_ptr<AudioParam>
AudioParam::exponentialRampToValueAtTime(float value, double endTime) {
  if (value == 0) {
    throw DOMException(
        "AudioParam: exponentialRampToValueAtTime parameter 'value' must be "
        "non-zero",
        "RangeError");
  }

  if (endTime < 0 || std::isnan(endTime) || std::isinf(endTime)) {
    throw DOMException(
        "AudioParam: exponentialRampToValueAtTime parameter 'endTime' must be "
        "non-negative",
        "RangeError");
  }

  auto currentTime = context_.lock()->getCurrentTime();
  endTime = std::max(endTime, currentTime);

  events_.emplace(
      details::ParamEventExponentialRamp{eventIndex_, value, endTime});
  return shared_from_this();
}

std::shared_ptr<AudioParam> AudioParam::setTargetAtTime(float target,
                                                        double startTime,
                                                        float timeConstant) {
  if (startTime < 0 || std::isnan(startTime) || std::isinf(startTime)) {
    throw DOMException("AudioParam: setTargetAtTime parameter 'startTime' must "
                       "be non-negative",
                       "RangeError");
  }

  if (timeConstant < 0) {
    throw DOMException("AudioParam: setTargetAtTime parameter 'timeConstant' "
                       "must be non-negative",
                       "RangeError");
  }

  events_.emplace(details::ParamEventSetTarget{
      eventIndex_,
      target,
      startTime,
      timeConstant,
  });
  return shared_from_this();
}

std::shared_ptr<AudioParam>
AudioParam::setValueCurveAtTime(const std::vector<float> &values,
                                double startTime, double duration) {
  if (values.size() < 2) {
    throw DOMException(
        "AudioParam: setValueCurveAtTime parameter 'values' must contain at "
        "least two values",
        "InvalidStateError");
  }

  if (startTime < 0 || std::isnan(startTime) || std::isinf(startTime)) {
    throw DOMException("AudioParam: setValueCurveAtTime parameter 'startTime' "
                       "must be non-negative",
                       "RangeError");
  }

  startTime = std::max(startTime, context_.lock()->getCurrentTime());

  if (duration <= 0 || std::isnan(duration) || std::isinf(duration)) {
    throw DOMException("AudioParam: setValueCurveAtTime parameter 'duration' "
                       "must be greater than 0",
                       "RangeError");
  }

  events_.emplace(details::ParamEventSetValueCurve{
      eventIndex_, values, startTime, duration, std::nullopt});

  return shared_from_this();
}

std::shared_ptr<AudioParam>
AudioParam::cancelScheduledValues(double cancelTime) {
  if (cancelTime < 0) {
    throw DOMException(
        "AudioParam: cancelScheduledValues parameter 'cancelTime' must be "
        "non-negative",
        "RangeError");
  }

  cancelTime = std::max(cancelTime, context_.lock()->getCurrentTime());
  auto it = events_.lower_bound(details::ParamEventSetValue{0, 0, cancelTime});
  events_.erase(it, events_.end());
  return shared_from_this();
}

std::shared_ptr<AudioParam> AudioParam::cancelAndHoldAtTime(double cancelTime) {
  if (cancelTime < 0) {
    throw DOMException(
        "AudioParam: cancelAndHoldAtTime parameter 'cancelTime' must be "
        "non-negative",
        "RangeError");
  }

  auto e1 = floorEvent(cancelTime);
  auto e2 = higherEvent(cancelTime);

  // TODO

  return shared_from_this();
}

std::set<details::ParamEvent, details::ParamEventLess>::iterator
AudioParam::floorEvent(double time) {
  auto it = events_.upper_bound(details::ParamEventSetValue{0, 0, time});

  if (it == events_.begin()) {
    return events_.end();
  }

  --it;
  return it;
}

std::set<details::ParamEvent, details::ParamEventLess>::iterator
AudioParam::higherEvent(double time) {
  return events_.upper_bound(details::ParamEventSetValue{0, 0, time});
}

float AudioParam::getLastValue(
    std::set<details::ParamEvent, details::ParamEventLess>::iterator event)
    const {
  if (event == events_.end()) {
    return defaultValue_;
  }

  if (std::holds_alternative<details::ParamEventSetValue>(*event)) {
    return std::get<details::ParamEventSetValue>(*event).value;
  }

  if (std::holds_alternative<details::ParamEventLinearRamp>(*event)) {
    return std::get<details::ParamEventLinearRamp>(*event).value;
  }

  if (std::holds_alternative<details::ParamEventExponentialRamp>(*event)) {
    return std::get<details::ParamEventExponentialRamp>(*event).value;
  }

  if (std::holds_alternative<details::ParamEventSetTarget>(*event)) {
    auto &e = std::get<details::ParamEventSetTarget>(*event);
    float prevValue;

    if (event == events_.begin()) {
      prevValue = defaultValue_;
    } else {
      auto prev = std::prev(event);
      prevValue = getLastValue(prev);
    }

    auto nextEvent = std::next(event);
    double nextTime;

    if (nextEvent == events_.end()) {
      // No next event, asymptotically approach the target
      return e.target;
    } else {
      nextTime =
          std::visit([](const auto &x) { return x.getTime(); }, *nextEvent);
    }

    return e.target + (prevValue - e.target) *
                          std::exp((e.startTime - nextTime) / e.timeConstant);
  }

  if (std::holds_alternative<details::ParamEventSetValueCurve>(*event)) {
    auto &e = std::get<details::ParamEventSetValueCurve>(*event);
    return e.values.back();
  }

  return currentValue_;
}
} // namespace web_audio
#endif