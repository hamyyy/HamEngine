#include "HamLayer.h"

#include "Ham/Core/Math.h"
#include "Ham/Script/CameraController.h"
#include "Ham/Script/Oscillate.h"
#include "Ham/Util/ImGuiExtra.h"
#include "Ham/Parser/STLParser.h"

#include <sol/sol.hpp>

namespace Ham {

std::vector<Component::VertexData> getCubeVertices()
{
  std::vector<Component::VertexData> vertices = {
      // Front Face
      {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
      {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
      {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
      {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},

      // Right Face
      {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},

      // Back Face
      {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
      {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
      {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
      {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},

      // Left Face
      {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
      {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
      {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
      {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},

      // Top Face
      {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
      {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
      {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},

      // Bottom Face
      {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
      {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
      {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
      {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}}};

  return vertices;
}

std::vector<unsigned int> getCubeIndices()
{
  std::vector<unsigned int> indices =
      {
          0, 2, 1, 2, 0, 3,        // Front Face
          4, 6, 5, 6, 4, 7,        // Right Face
          8, 10, 9, 10, 8, 11,     // Back Face
          12, 14, 13, 14, 12, 15,  // Left Face
          16, 18, 17, 18, 16, 19,  // Top Face
          20, 22, 21, 22, 20, 23   // Bottom Face
      };

  return indices;
}

std::vector<Component::VertexData> GetSphereVertices(float radius, int h_segments, int v_segments)
{
  std::vector<Component::VertexData> vertices;

  float sectorStep = 2 * math::pi<float> / h_segments;
  float stackStep = math::pi<float> / v_segments;

  for (int i = 0; i <= v_segments; ++i) {
    float stackAngle = math::pi<float> / 2 - i * stackStep;
    float xy = radius * cosf(stackAngle);
    float z = radius * sinf(stackAngle);

    for (int j = 0; j <= h_segments; ++j) {
      float sectorAngle = j * sectorStep;

      float x = xy * cosf(sectorAngle);
      float y = xy * sinf(sectorAngle);

      math::vec3 position(x, y, z);
      math::vec3 normal(math::normalize(position));

      vertices.push_back({position, normal});
    }
  }

  return vertices;
}

std::vector<unsigned int> GetSphereIndices(int h_segments, int v_segments)
{
  std::vector<unsigned int> indices;

  int numStacks = v_segments + 1;
  int numSectors = h_segments + 1;

  for (int i = 0; i < numStacks; ++i) {
    int k1 = i * numSectors;        // current stack
    int k2 = (i + 1) * numSectors;  // next stack

    for (int j = 0; j < numSectors; ++j) {
      if (i != 0) {
        indices.push_back(k1 + j);
        indices.push_back(k2 + j);
        indices.push_back(k1 + j + 1);
      }

      if (i != numStacks - 1) {
        indices.push_back(k1 + j + 1);
        indices.push_back(k2 + j);
        indices.push_back(k2 + j + 1);
      }
    }
  }

  return indices;
}

void CalculateNormals(std::vector<Component::VertexData> &vertices, const std::vector<unsigned int> &indices)
{
  // Initialize normals of all vertices to zero vectors
  for (auto &vertex : vertices) {
    vertex.Normal = math::vec3(0.0f);
  }

  // Calculate face normals and accumulate to vertex normals
  for (size_t i = 0; i < indices.size(); i += 3) {
    unsigned int index1 = indices[i];
    unsigned int index2 = indices[i + 1];
    unsigned int index3 = indices[i + 2];

    auto &vertex1 = vertices[index1];
    auto &vertex2 = vertices[index2];
    auto &vertex3 = vertices[index3];

    const math::vec3 &v1 = vertex1.Position;
    const math::vec3 &v2 = vertex2.Position;
    const math::vec3 &v3 = vertex3.Position;

    math::vec3 faceNormal = math::normalize(math::cross(v2 - v1, v3 - v1));

    vertex1.Normal += faceNormal;
    vertex2.Normal += faceNormal;
    vertex3.Normal += faceNormal;
  }

  // Normalize the vertex normals
  for (auto &vertex : vertices) {
    vertex.Normal = math::normalize(vertex.Normal);
  }
}

HamLayer::HamLayer(Application *app) : Layer("HamLayer"), m_App(app), m_Scene(m_App->GetScene()) {}

HamLayer::~HamLayer() {}

void HamLayer::OnAttach()
{
  {
    auto cameraEntity = m_Scene.CreateEntity("Camera");
    cameraEntity.AddComponent<Component::Camera>();
    cameraEntity.GetComponent<Component::Transform>() = Component::Transform(math::inverse(math::lookAt(math::vec3(3.0f, 3.0f, 3.0f), math::zero<math::vec3>())));  // Component::Transform(math::inverse(math::camera()) * math::translate((math::camera() * math::vec4(math::backward(), 0.0f)).xyz * 10.0f));

    auto &scriptList = cameraEntity.AddComponent<Component::NativeScriptList>();
    scriptList.AddScript<CameraController>("CameraController");
  }

  {
    auto entity = m_Scene.CreateEntity("Sphere");
    entity.GetComponent<Component::Transform>().Position = math::vec3(2.0f, 0.0f, 0.0f);
    auto &shaders = entity.GetComponent<Component::ShaderList>();
    shaders.Add("face-normal");
    auto &mesh = entity.AddComponent<Component::Mesh>(GetSphereVertices(0.5, 32, 32), GetSphereIndices(32, 32));

    // mesh.Indicies.Bind();
    // mesh.Verticies.Bind();

    mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Position));
    mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Normal));

    mesh.ShowWireframe = true;

    auto &scriptList = entity.AddComponent<Component::NativeScriptList>();
    scriptList.AddScript<Oscillate>("Oscillate");

    // mesh.VAO.Unbind();
    // mesh.Verticies.Unbind();
    // mesh.Indicies.Unbind();
  }

  {
    auto entity = m_Scene.CreateEntity("Monkey");
    auto &shaders = entity.GetComponent<Component::ShaderList>();
    // shaders.Add("funk");
    // shaders.Add("outline");
    shaders.Add("face-normal");
    std::vector<Component::VertexData> vertices;
    std::vector<unsigned int> indices;
    fs::ReadOBJFile(ASSETS_PATH "models/monkey.obj", vertices, indices);
    CalculateNormals(vertices, indices);

    auto &mesh = entity.AddComponent<Component::Mesh>(vertices, indices);

    // mesh.Indicies.Bind();
    // mesh.Verticies.Bind();

    mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Position));
    mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Normal));

    // mesh.VAO.Unbind();
    // mesh.Verticies.Unbind();
    // mesh.Indicies.Unbind();

    // m_Scene.SetSelectedEntity(entity);
  }

  auto cubesParent = m_Scene.CreateEntity("Cubes");

  // int width = 50;
  // int height = 50;
  // for (int y = -height / 2; y < height / 2; y++)
  //     for (int x = -width / 2; x < width / 2; x++)
  //     {
  //         auto entity = m_Scene.CreateEntity("Cube-" + std::to_string(x) + "-" + std::to_string(y));
  //         // entity.GetComponent<Component::Transform>().Position = math::vec3(-2.0f, 0.0f, 0.0f);

  //         // z as a function of x and y
  //         float z = 10.0f * sin(0.15f * x) * cos(0.15f * y);

  //         entity.GetComponent<Component::Transform>().Position = math::vec3(x, y, z);
  //         auto &shaders = entity.GetComponent<Component::ShaderList>();
  //         shaders.Add("face-normal");
  //         shaders.Add("vertex-normal");

  //         auto &mesh = entity.AddComponent<Component::Mesh>(getCubeVertices(), getCubeIndices());

  //         // mesh.Indicies.Bind();
  //         // mesh.Verticies.Bind();

  //         mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Position));
  //         mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Normal));

  //         // mesh.VAO.Unbind();
  //         // mesh.Verticies.Unbind();
  //         // mesh.Indicies.Unbind();

  //         auto &scriptList = entity.AddComponent<Component::NativeScriptList>();
  //         scriptList.AddScript<Oscillate>("Oscillate");

  //         entity.SetParent(cubesParent);
  //     }

  {
    auto entity = m_Scene.CreateEntity("Cube");
    entity.GetComponent<Component::Transform>().Position = math::vec3(-2.0f, 0.0f, 0.0f);

    // z as a function of x and y

    auto &shaders = entity.GetComponent<Component::ShaderList>();
    shaders.Add("face-normal");

    auto &mesh = entity.AddComponent<Component::Mesh>(getCubeVertices(), getCubeIndices());

    // mesh.Indicies.Bind();
    // mesh.Verticies.Bind();

    mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Position));
    mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Normal));

    // mesh.VAO.Unbind();
    // mesh.Verticies.Unbind();
    // mesh.Indicies.Unbind();

    auto &scriptList = entity.AddComponent<Component::NativeScriptList>();
    scriptList.AddScript<Oscillate>("Oscillate");

    entity.SetParent(cubesParent);
  }

  // {
  //     auto entity = m_Scene.CreateEntity("Living Room");
  //     auto &shaders = entity.GetComponent<Component::ShaderList>();
  //     shaders.Add("face-normal");
  //     shaders.Add("vertex-normal");
  //     std::vector<Ham::Component::VertexData> vertices;
  //     std::vector<unsigned int> indices;
  //     CalculateNormals(vertices, indices);
  //     ReadOBJFile(ASSETS_PATH + "models/InteriorTest.obj", vertices, indices);

  //     auto &mesh = entity.AddComponent<Component::Mesh>(vertices, indices);

  //     // mesh.Indicies.Bind();
  //     // mesh.Verticies.Bind();

  //     mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Position));
  //     mesh.Vertices.DefineAttribute3f(offsetof(Component::VertexData, Normal));

  //     // mesh.VAO.Unbind();
  //     // mesh.Verticies.Unbind();
  //     // mesh.Indicies.Unbind();
  // }
}

void HamLayer::OnDetach() {}

void HamLayer::OnUpdate(TimeStep deltaTime)
{
}

void HamLayer::OnUIRender(TimeStep deltaTime)
{
}

bool HamLayer::OnEvent(const Events::Event &event)
{
  auto &scriptList = GetActiveCamera().GetComponent<Component::NativeScriptList>();
  auto script = scriptList.GetScript<CameraController>();

  if (event.Is<Events::MouseDragged>()) {
    auto &e = event.As<Events::MouseDragged>();

    if (script) {
      script->OnMouseDragged(e);
      return true;
    }
  }

  return false;
}

// void HamLayer::OnEvent(Event &event) {}

}  // namespace Ham