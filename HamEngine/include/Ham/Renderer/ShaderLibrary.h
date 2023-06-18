#pragma once

#include "Ham/Core/Base.h"
#include "Ham/Renderer/Shader.h"

namespace Ham
{
    class ShaderLibrary
    {
    public:
        static void Init();
        static std::shared_ptr<Shader> Load(std::string name, std::string vertexPath, std::string fragmentPath, std::string geometryPath = "");
        static std::shared_ptr<Shader> Get(std::string name);

    private:
        static std::unordered_map<std::string, std::shared_ptr<Shader>> s_Shaders;
    };
} // namespace Ham
