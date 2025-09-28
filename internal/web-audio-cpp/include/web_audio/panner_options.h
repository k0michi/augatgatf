#pragma once

#include "audio_node_options.h"
#include "distance_model_type.h"
#include "panning_model_type.h"

namespace web_audio {
struct PannerOptions : public AudioNodeOptions {
  PanningModelType panningModel = PanningModelType::eEqualpower;
  DistanceModelType distanceModel = DistanceModelType::eInverse;
  float positionX = 0;
  float positionY = 0;
  float positionZ = 0;
  float orientationX = 1;
  float orientationY = 0;
  float orientationZ = 0;
  double refDistance = 1;
  double maxDistance = 10000;
  double rolloffFactor = 1;
  double coneInnerAngle = 360;
  double coneOuterAngle = 360;
  double coneOuterGain = 0;
};
} // namespace web_audio