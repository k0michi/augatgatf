#pragma once

#include <string>

#include "automation_rate.h"

namespace web_audio {
struct AudioParamDescriptor {
  std::string name;
  float defaultValue = 0;
  float minValue = -3.4028235e38;
  float maxValue = 3.4028235e38;
  AutomationRate automationRate = AutomationRate::eARate;
};
} // namespace web_audio