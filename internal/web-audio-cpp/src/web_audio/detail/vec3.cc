#include "web_audio/detail/vec3.hh"

#include <cmath>

namespace web_audio::detail {
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vec3::dot(const Vec3 &v) const {
  return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
}

Vec3 Vec3::cross(const Vec3 &v) const {
  return Vec3((this->y * v.z) - (this->z * v.y),
              (this->z * v.x) - (this->x * v.z),
              (this->x * v.y) - (this->y * v.x));
}

Vec3 Vec3::diff(const Vec3 &v) const {
  return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
}

float Vec3::getMagnitude() const { return std::sqrt(dot(*this)); }

Vec3 Vec3::scale(float s) const {
  return Vec3(this->x * s, this->y * s, this->z * s);
}

Vec3 Vec3::normalize() const {
  float mag = getMagnitude();

  if (mag == 0) {
    return Vec3(0, 0, 0);
  }

  return scale(1.0f / mag);
}
} // namespace web_audio::detail