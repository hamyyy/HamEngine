#pragma once

#include "Ham/Util/File.h"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace Ham
{
    class Shader
    {
    public:
        Shader(std::string vertexPath, std::string fragmentPath);
        ~Shader();

        void Bind() const;
        void Unbind() const;
        void Reload();

        void SetUniform1i(const std::string &name, int value);
        void SetUniform2i(const std::string &name, glm::ivec2 value);
        void SetUniform3i(const std::string &name, glm::ivec3 value);
        void SetUniform4i(const std::string &name, glm::ivec4 value);

        void SetUniform1f(const std::string &name, float value);
        void SetUniform2f(const std::string &name, glm::vec2 value);
        void SetUniform3f(const std::string &name, glm::vec3 value);
        void SetUniform4f(const std::string &name, glm::vec4 value);

        void SetUniformMat3f(const std::string &name, const glm::mat3 &matrix);
        void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);

    private:
        unsigned int m_RendererID = -1;
        std::unordered_map<std::string, int> m_UniformLocationCache;

        unsigned int CompileShader(unsigned int type, const std::string &source);
        unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
        int GetUniformLocation(const std::string &name);

        std::string m_VertexSourcePath;
        std::string m_FragmentSourcePath;
    };
} // namespace Ham
