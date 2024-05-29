#pragma once

#include "Ham/Core/Math.h"
#include "Ham/Util/TimeStep.h"
#include "Ham/Util/UUID.h"
#include "Ham/Renderer/Buffer.h"
#include "Ham/Renderer/Shader.h"
#include "Ham/Renderer/ShaderLibrary.h"

#include <string>
#include <vector>
#include <functional>

namespace Ham {
class Entity;
class Scene;
struct NativeScript {
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
}  // namespace Ham

namespace Ham::Component {
using UUID = Ham::UUID;
using Parent = Ham::Entity;

struct Tag {
  std::string Name;
  uint32_t Order = 0;

  Tag() : Name("Unnamed") {}
  Tag(std::string name) : Name(name) {}
  Tag(const Tag &other) : Name(other.Name) {}

  operator std::string() { return Name; }
  operator std::string() const { return Name; }
  operator const char *() const { return Name.c_str(); }
};

struct Camera {
  math::mat4 Projection;
  float FOV = math::radians(45.0f);
  float AspectRatio = 1.0f;
  float Near = 0.001f;
  float Far = 2000.0f;
  bool Active = true;
  int Priority = 0;

  Camera(float fov, float aspectRatio, float near, float far) : FOV(fov), AspectRatio(aspectRatio), Near(near), Far(far), Projection(math::perspective(fov, {aspectRatio, 1.0f}, near, far)) {}
  Camera() : Projection(math::perspective(FOV, {AspectRatio, 1.0f}, Near, Far)) {}
  Camera(const Camera &other) : Projection(other.Projection) {}

  void Update()
  {
    Projection = math::perspective(FOV, {AspectRatio, 1.0f}, Near, Far);
  }

  void Update(float width, float height)
  {
    AspectRatio = width / height;
    Update();
  }
};

struct Transform {
  math::vec3 Position = {0.0f, 0.0f, 0.0f};
  math::vec3 Rotation = {0.0f, 0.0f, 0.0f};
  math::vec3 Scale = {1.0f, 1.0f, 1.0f};

  Transform() = default;
  Transform(math::vec3 pos, math::vec3 rot, math::vec3 scale) : Position(pos), Rotation(rot), Scale(scale) {}
  Transform(const Transform &other) : Position(other.Position), Rotation(other.Rotation), Scale(other.Scale) {}
  Transform(const math::mat4 &value)
  {
    auto t = Transform::FromMatrix(value);
    Position = t.Position;
    Rotation = t.Rotation;
    Scale = t.Scale;
  }

  operator math::mat4() { return ToMatrix(); }
  math::mat4 ToMatrix()
  {
    math::mat4 translation = math::translate(Position);
    math::mat4 rotation = math::rotateEuler(Rotation);
    math::mat4 scale = math::scale(Scale);

    return translation * rotation * scale;
  }

  static Transform FromMatrix(math::mat4 mat)
  {
    math::vec3 position;
    math::quat rotation;
    math::vec3 scale;
    math::decompose(mat, position, rotation, scale);
    return {position, math::euler(rotation), scale};
  }

  math::vec3 up()
  {
    return math::normalize(math::vec3(ToMatrix() * math::vec4(math::up(), 1.0f)));
  }

  math::vec3 down() { return -up(); }

  math::vec3 right()
  {
    return math::normalize(math::vec3(ToMatrix() * math::vec4(math::right(), 1.0f)));
  }

  math::vec3 left() { return -right(); }

  math::vec3 forward()
  {
    return math::normalize(math::vec3(ToMatrix() * math::vec4(math::forward(), 1.0f)));
  }

  math::vec3 backward() { return -forward(); }
};

struct ShaderList {
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

struct NativeScriptList {
  std::vector<std::shared_ptr<NativeScript>> Scripts;
  NativeScriptList() {}
  NativeScriptList(const NativeScriptList &other) : Scripts(other.Scripts) {}

  template <typename T, typename... Args>
  T *AddScript(Args &&...args)
  {
    if (HasScript<T>()) {
      HAM_CORE_ERROR("Script {0} already attached to this entity!", typeid(T).name());
      return GetScript<T>();
    }
    else {
      Scripts.push_back(std::make_shared<T>(std::forward<Args>(args)...));
      return &static_cast<T &>(*Scripts.back());
    }
  }

  template <typename T>
  T *GetScript()
  {
    for (auto &script : Scripts) {
      if (dynamic_cast<T *>(script.get()) != nullptr) {
        return &static_cast<T &>(*script);
      }
    }

    HAM_CORE_ERROR("Script {0} not found!", typeid(T).name());
    return nullptr;
  }

  template <typename T>
  bool HasScript()
  {
    for (auto &script : Scripts) {
      if (dynamic_cast<T *>(script.get()) != nullptr) {
        return true;
      }
    }

    return false;
  }
};

struct EntityList {
  std::vector<Entity> Entities;
  EntityList() {}
  EntityList(const EntityList &other) : Entities(other.Entities) {}

  void Add(Entity entity);
  void Remove(Entity entity);
  bool Contains(Entity entity);
};

struct VertexData {
  math::vec3 Position;
  math::vec3 Normal;
  // math::vec2 TexCoord;
};

struct Mesh {
  VertexBuffer<VertexData> Vertices;
  IndexBuffer Indices;
  VertexArray VAO;

  bool ShowWireframe = false;
  bool ShowFill = true;
  bool AlphaBlending = false;
  bool BackfaceCulling = true;

  Mesh() {}
  Mesh(const Mesh &other) : Vertices(other.Vertices), Indices(other.Indices) {}  // VBO(other.VBO)
  Mesh(const std::vector<VertexData> &verticies, const std::vector<uint32_t> &indicies)
  {
    Recalculate(verticies, indicies);
  }

  void Recalculate(const std::vector<VertexData> &verticies, const std::vector<uint32_t> &indicies)
  {
    if (!VAO.IsInitialized()) {
      VAO.Create();
    }
    VAO.Bind();

    if (!Indices.IsInitialized()) {
      Indices.Create();
    }
    Indices.Bind();
    Indices.SetData(indicies);

    if (!Vertices.IsInitialized()) {
      Vertices.Create();
    }
    Vertices.Bind();
    Vertices.SetData(verticies);

    // VBO.Unbind();
    // Verticies.Unbind();
    // Indicies.Unbind();
  }
};

}  // namespace Ham::Component
