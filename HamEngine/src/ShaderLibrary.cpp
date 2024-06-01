#include "Ham/Renderer/ShaderLibrary.h"
#include "Ham/Core/FileSystem.h"

#include "fmt/format.h"
#include <filesystem>

namespace Ham {
std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::s_Shaders;

void ShaderLibrary::Init()
{
  ShaderLibrary::Load("face-normal", ASSETS_PATH_CORE "shaders/default.vert", ASSETS_PATH_CORE "shaders/default.frag");
  ShaderLibrary::Load("outline", ASSETS_PATH_CORE "shaders/outline.vert", ASSETS_PATH_CORE "shaders/outline.frag");
  ShaderLibrary::Load("vertex-normal", ASSETS_PATH_CORE "shaders/default.vert", ASSETS_PATH_CORE "shaders/flat.frag", ASSETS_PATH_CORE "shaders/normals.geom");
  ShaderLibrary::Load("funk", ASSETS_PATH_CORE "shaders/default.vert", ASSETS_PATH_CORE "shaders/funk.frag");
  ShaderLibrary::Load("object-picker", ASSETS_PATH_CORE "shaders/default.vert", ASSETS_PATH_CORE "shaders/object-picker.frag");
}

std::shared_ptr<Shader> ShaderLibrary::Load(std::string name, std::string vertexPath, std::string fragmentPath, std::string geometryPath)
{
  HAM_CORE_ASSERT(s_Shaders.find(name) == s_Shaders.end(), "Shader already exists");
  HAM_CORE_ASSERT(std::filesystem::exists(vertexPath), fmt::format("Cannot find vertex shader: {0}", vertexPath));
  HAM_CORE_ASSERT(std::filesystem::exists(fragmentPath), fmt::format("Cannot find fragment shader: {0}", fragmentPath));
  if (geometryPath != "")
    HAM_CORE_ASSERT(std::filesystem::exists(geometryPath), "Geometry shader path is invalid");

  s_Shaders[name] = std::make_shared<Shader>(vertexPath, fragmentPath, geometryPath);
  return s_Shaders[name];
}

std::shared_ptr<Shader> ShaderLibrary::Get(std::string name)
{
  if (s_Shaders.find(name) == s_Shaders.end())
    return nullptr;
  return s_Shaders[name];
}
}  // namespace Ham
