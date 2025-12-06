#include "kl/geometry/sphere.hh"
#include <gtest/gtest.h>

using namespace kl::geometry;

TEST(Sphere2, AreaCircumference) {
  Circle<float> c{{0, 0}, 2.0f};
  EXPECT_NEAR(c.area(), 3.14159265f * 4.0f, 1e-5f);
  EXPECT_NEAR(c.circumference(), 2 * 3.14159265f * 2.0f, 1e-5f);
}

TEST(Sphere2, ContainsPoint) {
  Circle<float> c{{0, 0}, 1.0f};
  EXPECT_EQ(c.contains(kl::math::Vector<float, 2>{0.5f, 0.0f}),
            Containment::eContains);
  EXPECT_EQ(c.contains(kl::math::Vector<float, 2>{1.0f, 0.0f}),
            Containment::eIntersects);
  EXPECT_EQ(c.contains(kl::math::Vector<float, 2>{2.0f, 0.0f}),
            Containment::eDisjoint);
}

TEST(Sphere2, IntersectsCircle) {
  Circle<float> c1{{0, 0}, 1.0f};
  Circle<float> c2{{1.5f, 0}, 1.0f};
  EXPECT_TRUE(c1.intersects(c2));
  Circle<float> c3{{3.0f, 0}, 1.0f};
  EXPECT_FALSE(c1.intersects(c3));
}

TEST(Sphere3, VolumeSurface) {
  Sphere<float, 3> s{{0, 0, 0}, 1.0f};
  EXPECT_NEAR(s.volume(), 4.0f / 3.0f * 3.14159265f, 1e-5f);
  EXPECT_NEAR(s.surfaceArea(), 4.0f * 3.14159265f, 1e-5f);
}

TEST(Sphere3, ContainsPoint) {
  Sphere<float, 3> s{{0, 0, 0}, 1.0f};
  EXPECT_EQ(s.contains(kl::math::Vector<float, 3>{0.5f, 0.0f, 0.0f}),
            Containment::eContains);
  EXPECT_EQ(s.contains(kl::math::Vector<float, 3>{1.0f, 0.0f, 0.0f}),
            Containment::eIntersects);
  EXPECT_EQ(s.contains(kl::math::Vector<float, 3>{2.0f, 0.0f, 0.0f}),
            Containment::eDisjoint);
}

TEST(Sphere3, IntersectsSphere) {
  Sphere<float, 3> s1{{0, 0, 0}, 1.0f};
  Sphere<float, 3> s2{{1.5f, 0, 0}, 1.0f};
  EXPECT_TRUE(s1.intersects(s2));
  Sphere<float, 3> s3{{3.0f, 0, 0}, 1.0f};
  EXPECT_FALSE(s1.intersects(s3));
}
