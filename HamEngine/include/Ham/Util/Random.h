#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <random>
#include <chrono>

namespace Ham
{

    class Random
    {
    public:
        static void Init(uint32_t seed = -1);

        static float Float();
        static float Float(float min, float max);

        static int Int();
        static int Int(int min, int max);

        static bool Bool();

        static glm::vec2 Vec2();
        static glm::vec2 Vec2Dir();

        static glm::vec3 Vec3();
        static glm::vec3 Vec3Dir();

        static glm::vec4 Vec4();
        static glm::vec4 Vec4Dir();

        static glm::quat Quat();

    private:
        static std::mt19937 s_RandomEngine;
        static std::uniform_real_distribution<float> s_Distribution;
    };

} // namespace Ham::Random
