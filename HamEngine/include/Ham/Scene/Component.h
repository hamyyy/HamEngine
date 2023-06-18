#pragma once

#include "Ham/Util/TimeStep.h"
#include "Ham/Util/UUID.h"
#include "Ham/Renderer/Buffer.h"
#include "Ham/Renderer/Shader.h"
#include "Ham/Renderer/ShaderLibrary.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Ham/Util/GlmExtra.h"

#include <string>
#include <vector>

namespace Ham
{
    class Entity;
    class Scene;
    struct NativeScript
    {
        std::string Name;
        UUID ID;
        Entity *GameObject;
        Scene *World;
        bool Active = false;

        NativeScript(const std::string &name);
        NativeScript(const NativeScript &other);

        virtual ~NativeScript() {}
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(TimeStep deltaTime) {}
        virtual void OnUIUpdate(TimeStep deltaTime) {}

        operator bool() const { return Active; }
    };
} // namespace Ham

namespace Ham::Component
{
    using UUID = Ham::UUID;
    using Parent = Ham::Entity;

    struct Tag
    {
        std::string Name;
        uint32_t Order = 0;

        Tag() : Name("Unnamed") {}
        Tag(std::string name) : Name(name) {}
        Tag(const Tag &other) : Name(other.Name) {}

        operator std::string() { return Name; }
        operator std::string() const { return Name; }
        operator const char *() const { return Name.c_str(); }
    };

    struct Camera
    {
        glm::mat4 Projection;
        float FOV = glm::radians(45.0f);
        float AspectRatio = 1.0f;
        float Near = 0.001f;
        float Far = 2000.0f;
        bool Active = true;

        Camera(float fov, float aspectRatio, float near, float far) : FOV(fov), AspectRatio(aspectRatio), Near(near), Far(far), Projection(glm::perspective(fov, aspectRatio, near, far)) {}
        Camera() : Projection(glm::perspective(FOV, AspectRatio, Near, Far)) {}
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
        glm::vec3 Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

        Transform() = default;
        Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : Position(pos), Rotation(rot), Scale(scale) {}
        Transform(const Transform &other) : Position(other.Position), Rotation(other.Rotation), Scale(other.Scale) {}
        Transform(const glm::mat4 &value)
        {
            auto t = Transform::FromMatrix(value);
            Position = t.Position;
            Rotation = t.Rotation;
            Scale = t.Scale;
        }

        operator glm::mat4() { return ToMatrix(); }
        glm::mat4 ToMatrix()
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);

            return translation * rotation * scale;
        }

        static Transform FromMatrix(glm::mat4 mat)
        {
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 position;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(mat, scale, rotation, position, skew, perspective);
            return {position, glm::eulerAngles(rotation), scale};
        }

        glm::vec3 up()
        {
            return glm::normalize(glm::vec3(ToMatrix() * glm::vec4(glm::up(), 1.0f)));
        }

        glm::vec3 down() { return -up(); }

        glm::vec3 right()
        {
            return glm::normalize(glm::vec3(ToMatrix() * glm::vec4(glm::right(), 1.0f)));
        }

        glm::vec3 left() { return -right(); }

        glm::vec3 forward()
        {
            return glm::normalize(glm::vec3(ToMatrix() * glm::vec4(glm::forward(), 1.0f)));
        }

        glm::vec3 backward() { return -forward(); }
    };

    struct ShaderList
    {
        std::vector<std::string> Names;

        ShaderList() {}
        ShaderList(const ShaderList &other) : Names(other.Names) {}

        void Add(std::string name)
        {
            HAM_CORE_ASSERT(!Has(name), fmt::format("Shader {0} already added!", name));
            HAM_CORE_ASSERT(::Ham::ShaderLibrary::Get(name) != nullptr, fmt::format("Shader {0} not found!", name));
            Names.push_back(name);
        }

        void Remove(std::string name)
        {
            HAM_CORE_ASSERT(Has(name), fmt::format("Shader {0} not added!", name));
            Names.erase(std::remove(Names.begin(), Names.end(), name), Names.end());
        }

        std::shared_ptr<::Ham::Shader> Get(std::string name)
        {
            HAM_CORE_ASSERT(Has(name), fmt::format("Shader {0} not added!", name));
            return ::Ham::ShaderLibrary::Get(name);
        }

        bool Has(std::string name)
        {
            return std::find(Names.begin(), Names.end(), name) != Names.end();
        }
    };

    struct NativeScriptList
    {
        std::vector<std::shared_ptr<NativeScript>> Scripts;
        NativeScriptList() {}
        NativeScriptList(const NativeScriptList &other) : Scripts(other.Scripts) {}

        template <typename T, typename... Args>
        T *AddScript(Args &&...args)
        {
            if (HasScript<T>())
            {
                HAM_CORE_ERROR("Script {0} already attached to this entity!", typeid(T).name());
                return GetScript<T>();
            }
            else
            {
                Scripts.push_back(std::make_shared<T>(std::forward<Args>(args)...));
                return &static_cast<T &>(*Scripts.back());
            }
        }

        template <typename T>
        T *GetScript()
        {
            for (auto &script : Scripts)
            {
                if (dynamic_cast<T *>(script.get()) != nullptr)
                {
                    return &static_cast<T &>(*script);
                }
            }

            HAM_CORE_ERROR("Script {0} not found!", typeid(T).name());
            return nullptr;
        }

        template <typename T>
        bool HasScript()
        {
            for (auto &script : Scripts)
            {
                if (dynamic_cast<T *>(script.get()) != nullptr)
                {
                    return true;
                }
            }

            return false;
        }
    };

    struct EntityList
    {
        std::vector<Entity> Entities;
        EntityList() {}
        EntityList(const EntityList &other) : Entities(other.Entities) {}

        void Add(Entity entity);
        void Remove(Entity entity);
        bool Contains(Entity entity);
    };

    struct VertexData
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        // glm::vec2 TexCoord;
    };

    struct Mesh
    {
        VertexBuffer<VertexData> Vertices;
        IndexBuffer Indices;
        VertexArray VAO;

        bool ShowWireframe = false;
        bool ShowFill = true;
        bool AlphaBlending = false;
        bool BackfaceCulling = true;

        Mesh() {}
        Mesh(const Mesh &other) : Vertices(other.Vertices), Indices(other.Indices) {} // VBO(other.VBO)
        Mesh(const std::vector<VertexData> &verticies, const std::vector<uint32_t> &indicies)
        {
            VAO.Create();
            VAO.Bind();

            Indices.Create();
            Indices.Bind();
            Indices.SetData(indicies);

            Vertices.Create();
            Vertices.Bind();
            Vertices.SetData(verticies);

            // VBO.Unbind();
            // Verticies.Unbind();
            // Indicies.Unbind();
        }
    };

} // namespace Ham::Component
