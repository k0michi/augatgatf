#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "common.h"

namespace web_audio {
class AudioParam;
}

namespace web_audio::details {
class ParamCollection {
public:
  ParamCollection() = default;
  ~ParamCollection() noexcept = default;

  float getValue(std::shared_ptr<AudioParam> param, size_t frame) const;

  void setValue(std::shared_ptr<AudioParam> param, float value);

  void setValues(std::shared_ptr<AudioParam> param,
                 const std::vector<float> &values);

  void clear();

  WEB_AUDIO_PRIVATE
      : std::unordered_map<std::shared_ptr<AudioParam>, std::vector<float>>
            params_;
};
} // namespace web_audio::details