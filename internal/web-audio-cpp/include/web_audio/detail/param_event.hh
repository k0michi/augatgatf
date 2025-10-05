#pragma once

#include <cstdint>
#include <limits>
#include <optional>
#include <variant>
#include <vector>

namespace web_audio::detail {
struct ParamEventExponentialRamp {
  std::uint32_t index;
  float value;
  double endTime;

  double getTime() const { return endTime; }

  std::uint32_t getIndex() const { return index; }
};

struct ParamEventLinearRamp {
  std::uint32_t index;
  float value;
  double endTime;

  double getTime() const { return endTime; }

  std::uint32_t getIndex() const { return index; }
};

struct ParamEventSetTarget {
  std::uint32_t index;
  float target;
  double startTime;
  float timeConstant;

  double getTime() const { return startTime; }

  std::uint32_t getIndex() const { return index; }
};

struct ParamEventSetValue {
  std::uint32_t index;
  float value;
  double startTime;

  double getTime() const { return startTime; }

  std::uint32_t getIndex() const { return index; }
};

struct ParamEventSetValueCurve {
  std::uint32_t index;
  std::vector<float> values;
  double startTime;
  double duration;
  std::optional<double> canceledTime;

  double getTime() const { return startTime; }

  std::uint32_t getIndex() const { return index; }
};

using ParamEvent = std::variant<ParamEventExponentialRamp, ParamEventLinearRamp,
                                ParamEventSetTarget, ParamEventSetValue,
                                ParamEventSetValueCurve>;

struct ParamEventLess {
  bool operator()(const ParamEvent &a, const ParamEvent &b) const {
    return std::visit(
        [](const auto &x, const auto &y) {
          return x.getTime() < y.getTime() ||
                 (x.getTime() == y.getTime() && x.getIndex() < y.getIndex());
        },
        a, b);
  }
};
} // namespace web_audio::detail