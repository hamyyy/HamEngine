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

    math::vec2 Random::Vec2()
    {
        return math::vec2(Float(), Float());
    }

    math::vec2 Random::Vec2Dir()
    {
        return math::normalize(math::vec2(Float(-1, 1), Float(-1, 1)));
    }

    math::vec3 Random::Vec3()
    {
        return math::vec3(Float(), Float(), Float());
    }

    math::vec3 Random::Vec3Dir()
    {
        return math::normalize(math::vec3(Float(-1, 1), Float(-1, 1), Float(-1, 1)));
    }

    math::vec4 Random::Vec4()
    {
        return math::vec4(Float(), Float(), Float(), Float());
    }

    math::vec4 Random::Vec4Dir()
    {
        return math::normalize(math::vec4(Float(-1, 1), Float(-1, 1), Float(-1, 1), Float(-1, 1)));
    }

    math::quat Random::Quat()
    {
        return math::quat(Vec3Dir() * math::pi<float>);
    }
} // namespace Ham
