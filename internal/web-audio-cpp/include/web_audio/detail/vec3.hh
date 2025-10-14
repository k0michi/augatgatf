#pragma once

#include "common.hh"

namespace web_audio::detail {
class Vec3 {
public:
  Vec3(float x, float y, float z);

  float dot(const Vec3 &v) const;

  Vec3 cross(const Vec3 &v) const;

  Vec3 diff(const Vec3 &v) const;

  float getMagnitude() const;

  Vec3 scale(float s) const;

  Vec3 normalize() const;

  WEB_AUDIO_PRIVATE : float x;
  float y;
  float z;
};
} // namespace web_audio::detail
