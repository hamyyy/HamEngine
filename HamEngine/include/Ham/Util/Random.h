#pragma once

#include "Ham/Core/Math.h"

#include <random>
#include <chrono>

namespace Ham {

class Random {
 public:
  static void Init(uint32_t seed = -1);

  static float Float();
  static float Float(float min, float max);

  static int Int();
  static int Int(int min, int max);

  static bool Bool();

  static math::vec2 Vec2();
  static math::vec2 Vec2Dir();

  static math::vec3 Vec3();
  static math::vec3 Vec3Dir();

  static math::vec4 Vec4();
  static math::vec4 Vec4Dir();

  static math::quat Quat();

 private:
  static std::mt19937 s_RandomEngine;
  static std::uniform_real_distribution<float> s_Distribution;
};

}  // namespace Ham
