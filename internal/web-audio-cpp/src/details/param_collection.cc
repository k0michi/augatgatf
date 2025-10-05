#include "web_audio/details/param_collection.h"

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