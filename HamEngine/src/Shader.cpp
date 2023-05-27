#include "Ham/Renderer/Shader.h"

#include "Ham/Core/Base.h"

#include <glad/glad.h>

namespace Ham
{
    Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        m_RendererID = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        glAttachShader(m_RendererID, vs);
        glAttachShader(m_RendererID, fs);
        glLinkProgram(m_RendererID);
        glValidateProgram(m_RendererID);

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    void Shader::SetUniform1i(const std::string &name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniform1f(const std::string &name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
    {
        glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
    }

    void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
    }

    unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
    {
        unsigned int id = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        // Error handling
        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char *message = (char *)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            HAM_CORE_ERROR("Failed to compile {0} shader!", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
            HAM_CORE_ERROR("{0}", message);
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    int Shader::GetUniformLocation(const std::string &name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        int location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
            HAM_CORE_WARN("Uniform {0} doesn't exist!", name);
        m_UniformLocationCache[name] = location;
        return location;
    }
} // namespace Ham
