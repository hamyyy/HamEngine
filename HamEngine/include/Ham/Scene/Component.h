#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include <string>

namespace Ham::Component
{
    struct Tag
    {
        std::string Name;
        Tag() : Name("Unnamed") {}
        Tag(std::string name) : Name(name) {}
        Tag(const Tag &other) : Name(other.Name) {}
        operator std::string() const { return Name; }
        operator const char *() const { return Name.c_str(); }
    };

    inline static uint64_t s_IDCounter = 0;
    struct ID
    {
        uint64_t Value;
        ID() : Value(s_IDCounter++) {}
        ID(const ID &other) : Value(other.Value) {}
        operator uint64_t() const { return Value; }
    };

    struct Camera
    {
        glm::mat4 Projection;
        float FOV = glm::radians(45.0f);
        float AspectRatio = 1.0f;
        float Near = 0.01f;
        float Far = 100.0f;
        bool Active = true;

        Camera(float fov, float aspectRatio, float near, float far) : FOV(fov), AspectRatio(aspectRatio), Near(near), Far(far), Projection(glm::perspective(fov, aspectRatio, near, far)) {}
        Camera() : Projection(glm::perspective(glm::radians(45.0f), 1.0f, 0.001f, 1000.0f)) {}
        Camera(const Camera &other) : Projection(other.Projection) {}

        void Update()
        {
            Projection = glm::perspective(FOV, AspectRatio, Near, Far);
        }

        void Update(float width, float height)
        {
            AspectRatio = width / height;
            Update();
        }
    };

    struct Transform
    {
        glm::mat4 Value;

        Transform() : Value(glm::mat4(1.0f)) {}
        Transform(const Transform &other) : Value(other.Value) {}
        Transform(const glm::mat4 &value) : Value(value) {}

        operator glm::mat4() const { return Value; }
    };

} // namespace Ham::Compone
