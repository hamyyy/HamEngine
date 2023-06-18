#include "Ham/Util/Random.h"

#include <limits>

namespace Ham
{

    std::mt19937 Random::s_RandomEngine;
    std::uniform_real_distribution<float> Random::s_Distribution;

    float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    int lerp(int a, int b, float f)
    {
        return (int)(a + f * (b - a));
    }

    void Random::Init(uint32_t seed)
    {
        if (seed == -1)
            s_RandomEngine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        else
            s_RandomEngine.seed(seed);
    }

    float Random::Float()
    {
        return s_Distribution(s_RandomEngine);
    }

    float Random::Float(float min, float max)
    {
        return lerp(min, max, s_Distribution(s_RandomEngine));
    }

    bool Random::Bool()
    {
        return Float() > 0.5f;
    }

    glm::vec2 Random::Vec2()
    {
        return glm::vec2(Float(), Float());
    }

    glm::vec2 Random::Vec2Dir()
    {
        return glm::normalize(glm::vec2(Float(-1, 1), Float(-1, 1)));
    }

    glm::vec3 Random::Vec3()
    {
        return glm::vec3(Float(), Float(), Float());
    }

    glm::vec3 Random::Vec3Dir()
    {
        return glm::normalize(glm::vec3(Float(-1, 1), Float(-1, 1), Float(-1, 1)));
    }

    glm::vec4 Random::Vec4()
    {
        return glm::vec4(Float(), Float(), Float(), Float());
    }

    glm::vec4 Random::Vec4Dir()
    {
        return glm::normalize(glm::vec4(Float(-1, 1), Float(-1, 1), Float(-1, 1), Float(-1, 1)));
    }

    glm::quat Random::Quat()
    {
        return glm::quat(Vec3Dir() * glm::pi<float>());
    }
} // namespace Ham
