#pragma once

#include "Ham/Util/TimeStep.h"

#include <glm/glm.hpp>

namespace glm
{
    // static glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
    // static glm::vec3 down = glm::vec3(0.0f, 0.0f, -1.0f);
    // static glm::vec3 left = glm::vec3(-1.0f, 0.0f, 0.0f);
    // static glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    // static glm::vec3 forward = glm::vec3(0.0f, 1.0f, 0.0f);
    // static glm::vec3 backward = glm::vec3(0.0f, -1.0f, 0.0f);

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType up()
    {
        if constexpr (std::is_same_v<genType, glm::vec2>)
            return genType(0, 1);
        return genType(0, 0, 1);
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType down()
    {
        if constexpr (std::is_same_v<genType, glm::vec2>)
            return genType(0, -1);
        return genType(0, 0, -1);
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType left()
    {
        if constexpr (std::is_same_v<genType, glm::vec2>)
            return genType(-1, 0);
        return genType(-1, 0, 0);
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType right()
    {
        if constexpr (std::is_same_v<genType, glm::vec2>)
            return genType(1, 0);
        return genType(1, 0, 0);
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType forward()
    {
        return genType(0, 1, 0);
    }

    template <typename genType = glm::vec3>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType backward()
    {
        return genType(0, -1, 0);
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