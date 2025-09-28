#pragma once

#include "audio_node_options.h"

namespace web_audio {
struct DelayOptions : public AudioNodeOptions {
  double maxDelayTime = 1;
  double delayTime = 0;
};
} // namespace web_audio