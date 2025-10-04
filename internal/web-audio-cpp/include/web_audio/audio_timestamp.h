#pragma once

namespace web_audio {
using DOMHighResTimeStamp = double;

struct AudioTimestamp {
  double contextTime;
  DOMHighResTimeStamp performanceTime;
};
} // namespace web_audio