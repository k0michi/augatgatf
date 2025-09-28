#pragma once

#include "audio_node_options.h"

namespace web_audio {
struct StereoPannerOptions : public AudioNodeOptions {
  float pan = 0;
};
} // namespace web_audio