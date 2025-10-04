#pragma once

#include <limits>
#include <string>

#include "automation_rate.h"

namespace web_audio {
struct AudioParamDescriptor {
  std::string name;
  float defaultValue = 0;
  float minValue = std::numeric_limits<float>::lowest();
  float maxValue = std::numeric_limits<float>::max();
  AutomationRate automationRate = AutomationRate::eARate;
};
} // namespace web_audio