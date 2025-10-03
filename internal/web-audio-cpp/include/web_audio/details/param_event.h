#pragma once

#include <limits>
#include <optional>
#include <variant>
#include <vector>

namespace web_audio::details {
struct ParamEventExponentialRamp {
  float value;
  double endTime;

  double getEndTime() const { return endTime; }
};

struct ParamEventLinearRamp {
  float value;
  double endTime;

  double getEndTime() const { return endTime; }
};

struct ParamEventSetTarget {
  float target;
  double startTime;
  float timeConstant;

  double getEndTime() const { return startTime; }
};

struct ParamEventSetValue {
  float value;
  double startTime;

  double getEndTime() const { return startTime; }
};

struct ParamEventSetValueCurve {
  std::vector<float> values;
  double startTime;
  double duration;
  std::optional<double> canceledTime;

  double getBeginTime() const { return startTime; }

  double getEndTime() const {
    return canceledTime.value_or(startTime + duration);
  }
};

using ParamEvent = std::variant<ParamEventExponentialRamp, ParamEventLinearRamp,
                                ParamEventSetTarget, ParamEventSetValue,
                                ParamEventSetValueCurve>;

struct ParamEventLess {
  bool operator()(const ParamEvent &a, const ParamEvent &b) const {
    return std::visit(
        [](const auto &x, const auto &y) {
          return x.getEndTime() < y.getEndTime();
        },
        a, b);
  }
};
} // namespace web_audio::details