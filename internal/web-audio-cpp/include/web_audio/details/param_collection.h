#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

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

private:
  std::unordered_map<std::shared_ptr<AudioParam>, std::vector<float>> params_;
};
} // namespace web_audio::details

#ifdef WEB_AUDIO_IMPLEMENTATION
namespace web_audio::details {
float ParamCollection::getValue(std::shared_ptr<AudioParam> param,
                                size_t frame) const {
  auto it = params_.find(param);
  if (it == params_.end()) {
    return 0.0f;
  }

  const auto &values = it->second;
  if (frame < values.size()) {
    return values[frame];
  } else {
    return values.front();
  }
}

void ParamCollection::setValue(std::shared_ptr<AudioParam> param, float value) {
  params_[param] = {value};
}

void ParamCollection::setValues(std::shared_ptr<AudioParam> param,
                                const std::vector<float> &values) {
  params_[param] = values;
}

void ParamCollection::clear() { params_.clear(); }
} // namespace web_audio::details
#endif // WEB_AUDIO_IMPLEMENTATION