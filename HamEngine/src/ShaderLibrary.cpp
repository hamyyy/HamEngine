#include "Ham/Renderer/ShaderLibrary.h"

namespace Ham
{
    std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::s_Shaders;

    void ShaderLibrary::Init()
    {
        ShaderLibrary::Load("face-normal", ASSETS_PATH + "shaders/default.vert", ASSETS_PATH + "shaders/default.frag");
        ShaderLibrary::Load("vertex-normal", ASSETS_PATH + "shaders/default.vert", ASSETS_PATH + "shaders/flat.frag", ASSETS_PATH + "shaders/normals.geom");
        ShaderLibrary::Load("funk", ASSETS_PATH + "shaders/default.vert", ASSETS_PATH + "shaders/funk.frag");
        ShaderLibrary::Load("object-picker", ASSETS_PATH + "shaders/default.vert", ASSETS_PATH + "shaders/object-picker.frag");
    }

    std::shared_ptr<Shader> ShaderLibrary::Load(std::string name, std::string vertexPath, std::string fragmentPath, std::string geometryPath)
    {
        HAM_CORE_ASSERT(s_Shaders.find(name) == s_Shaders.end(), "Shader already exists");
        s_Shaders[name] = std::make_shared<Shader>(vertexPath, fragmentPath, geometryPath);
        return s_Shaders[name];
    }

    std::shared_ptr<Shader> ShaderLibrary::Get(std::string name)
    {
        if (s_Shaders.find(name) == s_Shaders.end())
            return nullptr;
        return s_Shaders[name];
    }
} // namespace Ham
