#include "web_audio/audio_param.hh"

#include "web_audio/base_audio_context.hh"

namespace web_audio {
std::shared_ptr<AudioParam> AudioParam::create(std::shared_ptr<AudioNode> owner,
                                               float defaultValue,
                                               float minValue, float maxValue,
                                               AutomationRate automationRate,
                                               bool allowARate) {
  auto instance = std::shared_ptr<AudioParam>(new AudioParam());
  // SPEC: Each AudioParam has an internal slot [[current value]], initially set
  // to the AudioParam's defaultValue.
  instance->currentValue_ = defaultValue;

  instance->owner_ = owner;
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
  auto context = getContext();
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

  checkValueCurve(startTime);

  events_.emplace(detail::ParamEventSetValue{eventIndex_++, value, startTime});
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

  checkValueCurve(endTime);

  events_.emplace(detail::ParamEventLinearRamp{eventIndex_++, value, endTime});
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

  checkValueCurve(endTime);

  auto currentTime = getContext()->getCurrentTime();
  endTime = std::max(endTime, currentTime);

  events_.emplace(
      detail::ParamEventExponentialRamp{eventIndex_++, value, endTime});
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

  checkValueCurve(startTime);

  events_.emplace(detail::ParamEventSetTarget{
      eventIndex_++,
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

  startTime = std::max(startTime, getContext()->getCurrentTime());

  if (duration <= 0 || std::isnan(duration) || std::isinf(duration)) {
    throw DOMException("AudioParam: setValueCurveAtTime parameter 'duration' "
                       "must be greater than 0",
                       "RangeError");
  }

  checkValueCurve(startTime);

  // SPEC: If setValueCurveAtTime() is called for time T and duration D and
  // there are any events having a time strictly greater than T, but strictly
  // less than T+D, then a NotSupportedError exception MUST be thrown.
  auto it = events_.upper_bound(detail::ParamEventSetValue{0, 0, startTime});

  if (it != events_.end()) {
    auto next = it;
    double nextTime =
        std::visit([](const auto &x) { return x.getTime(); }, *next);

    if (startTime < nextTime && nextTime < startTime + duration) {
      throw DOMException(
          "AudioParam: cannot schedule value curve overlapping with another "
          "event",
          "NotSupportedError");
    }
  }

  events_.emplace(detail::ParamEventSetValueCurve{
      eventIndex_++, values, startTime, duration, std::nullopt});
  events_.emplace(detail::ParamEventSetValue{eventIndex_++, values.back(),
                                             startTime + duration});

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

  cancelTime = std::max(cancelTime, getContext()->getCurrentTime());
  auto it = events_.lower_bound(detail::ParamEventSetValue{0, 0, cancelTime});
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

std::set<detail::ParamEvent, detail::ParamEventLess>::iterator
AudioParam::floorEvent(double time) {
  auto it = events_.upper_bound(detail::ParamEventSetValue{
      std::numeric_limits<std::uint32_t>::max(), 0, time});

  if (it == events_.begin()) {
    return events_.end();
  }

  --it;
  return it;
}

std::set<detail::ParamEvent, detail::ParamEventLess>::iterator
AudioParam::higherEvent(double time) {
  return events_.upper_bound(detail::ParamEventSetValue{
      std::numeric_limits<std::uint32_t>::max(), 0, time});
}

void AudioParam::checkValueCurve(double time) const {
  // SPEC: Similarly a NotSupportedError exception MUST be thrown if any
  // automation method is called at a time which is contained in [T,T+D), T
  // being the time of the curve and D its duration.

  auto it = events_.lower_bound(detail::ParamEventSetValue{0, 0, time});

  if (it != events_.begin()) {
    auto prev = std::prev(it);

    if (std::holds_alternative<detail::ParamEventSetValueCurve>(*prev)) {
      auto &e = std::get<detail::ParamEventSetValueCurve>(*prev);

      if (e.startTime <= time && time < e.startTime + e.duration) {
        throw DOMException(
            "AudioParam: cannot schedule event in the middle of a value curve",
            "NotSupportedError");
      }
    }
  }
}

float AudioParam::getEndValue(
    std::set<detail::ParamEvent, detail::ParamEventLess>::iterator event)
    const {
  if (event == events_.end()) {
    return defaultValue_;
  }

  if (std::holds_alternative<detail::ParamEventSetValue>(*event)) {
    return std::get<detail::ParamEventSetValue>(*event).value;
  }

  if (std::holds_alternative<detail::ParamEventLinearRamp>(*event)) {
    return std::get<detail::ParamEventLinearRamp>(*event).value;
  }

  if (std::holds_alternative<detail::ParamEventExponentialRamp>(*event)) {
    return std::get<detail::ParamEventExponentialRamp>(*event).value;
  }

  if (std::holds_alternative<detail::ParamEventSetTarget>(*event)) {
    auto &e = std::get<detail::ParamEventSetTarget>(*event);
    float prevValue;

    if (event == events_.begin()) {
      prevValue = defaultValue_;
    } else {
      auto prev = std::prev(event);
      prevValue = getEndValue(prev);
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

    return static_cast<float>(
        e.target + (prevValue - e.target) *
                       std::exp((e.startTime - nextTime) / e.timeConstant));
  }

  if (std::holds_alternative<detail::ParamEventSetValueCurve>(*event)) {
    auto &e = std::get<detail::ParamEventSetValueCurve>(*event);
    return e.values.back();
  }

  return currentValue_;
}

float AudioParam::getStartValue(
    std::set<detail::ParamEvent, detail::ParamEventLess>::iterator event)
    const {
  if (event == events_.begin()) {
    return defaultValue_;
  } else {
    return getEndValue(std::prev(event));
  }
}

void AudioParam::computeIntrinsicValues(double startTime,
                                        std::vector<float> &outputs) {
  auto context = getContext();
  auto sampleRate = context->getSampleRate();
  auto delta = 1.0 / sampleRate;

  for (uint32_t i = 0; i < outputs.size(); ++i) {
    auto time = startTime + i * delta;
    auto prevEvent = floorEvent(time);
    auto nextEvent = higherEvent(time);

    if (prevEvent == events_.end()) {
      outputs[i] = defaultValue_;
    } else if (std::holds_alternative<detail::ParamEventSetTarget>(
                   *prevEvent)) {
      auto &e = std::get<detail::ParamEventSetTarget>(*prevEvent);
      auto startValue = getStartValue(prevEvent);
      outputs[i] = static_cast<float>(
          e.target + (startValue - e.target) *
                         std::exp((e.startTime - time) / e.timeConstant));
    } else if (std::holds_alternative<detail::ParamEventSetValueCurve>(
                   *prevEvent)) {
      auto &e = std::get<detail::ParamEventSetValueCurve>(*prevEvent);
      auto k = static_cast<std::size_t>(std::floor(
          (e.values.size() - 1.0) / e.duration * (time - e.startTime)));

      if (k + 1 < e.values.size()) {
        auto t0 = e.startTime + e.duration * k / (e.values.size() - 1.0);
        auto t1 = e.startTime + e.duration * (k + 1) / (e.values.size() - 1.0);
        auto v0 = e.values[k];
        auto v1 = e.values[k + 1];
        outputs[i] = v0 + (v1 - v0) * (time - t0) / (t1 - t0);
      } else {
        outputs[i] = e.values.back();
      }
    } else {
      auto endValue = getEndValue(prevEvent);

      if (nextEvent == events_.end()) {
        outputs[i] = endValue;
      } else if (std::holds_alternative<detail::ParamEventLinearRamp>(
                     *nextEvent)) {
        auto prevTime =
            std::visit([](const auto &x) { return x.getTime(); }, *prevEvent);
        auto &e = std::get<detail::ParamEventLinearRamp>(*nextEvent);
        outputs[i] = endValue + (e.value - endValue) * (time - prevTime) /
                                    (e.endTime - prevTime);
      } else if (std::holds_alternative<detail::ParamEventExponentialRamp>(
                     *nextEvent)) {
        auto prevTime =
            std::visit([](const auto &x) { return x.getTime(); }, *prevEvent);
        auto &e = std::get<detail::ParamEventExponentialRamp>(*nextEvent);

        outputs[i] =
            endValue * std::pow(e.value / endValue,
                                (time - prevTime) / (e.endTime - prevTime));
      } else {
        outputs[i] = endValue;
      }
    }
  }
}

std::shared_ptr<AudioNode> AudioParam::getOwner() const {
  if (auto sp = owner_.lock()) {
    return sp;
  } else {
    throw std::runtime_error("AudioParam: owner has expired");
  }
}

std::shared_ptr<BaseAudioContext> AudioParam::getContext() const {
  return getOwner()->getContext();
}
} // namespace web_audio