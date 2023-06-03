#pragma once

#include "Ham/Util/TimeStep.h"

#include <glm/glm.hpp>

#if !defined(Y_UP) && !defined(Z_UP)
#define Z_UP
#endif

#if defined(Y_UP) && defined(Z_UP)
#error "Y_UP and Z_UP are both defined, please define only one of them"
#endif

namespace glm
{
    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType up()
    {
#if defined(Y_UP)
        if constexpr (std::is_same_v<genType, glm::vec2>)
            return genType(0, 1);
        return genType(0, 1, 0);
#elif defined(Z_UP)
        if constexpr (std::is_same_v<genType, glm::vec2>)
            return genType(0, 0, 1);
        return genType(0, 0, 1);
#else
#error "Y_UP and Z_UP are undefined, please define one of them"
#endif
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType down()
    {
        return -glm::up();
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType right()
    {
        if constexpr (std::is_same_v<genType, glm::vec2>)
            return genType(1, 0);
        return genType(1, 0, 0);
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType left()
    {
        return -glm::right();
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType forward()
    {
        return glm::cross(glm::up(), glm::right());
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType backward()
    {
        return -glm::forward();
    }

    GLM_FUNC_QUALIFIER mat4 camera()
    {
        return lookAt(glm::zero<vec3>(), glm::forward(), glm::up());
    }

    // add TimeStep operator

    // add Timestep operator
    static glm::vec2 operator*(const glm::vec2 &vec, const Ham::TimeStep &time)
    {
        return vec * (float)time;
    }

    static glm::vec3 operator*(const glm::vec3 &vec, const Ham::TimeStep &time)
    {
        return vec * (float)time;
    }
}