#include "Ham/Renderer/Shader.h"

#include "Ham/Core/Base.h"
#include "Ham/Util/Watcher.h"

#include <glad/gl.h>

namespace Ham
{

    std::vector<Shader *> Shader::s_Shaders;

    Shader::Shader(std::string vertexPath, std::string fragmentPath)
    {
        m_VertexSourcePath = vertexPath;
        m_FragmentSourcePath = fragmentPath;
        m_LastReloadTime = std::chrono::system_clock::now();

        auto onFileChanged = [this]()
        {
            Reload();
        };
        m_UnSubscribeFunctions.push_back(FileWatcher::Watch(m_VertexSourcePath, onFileChanged));
        m_UnSubscribeFunctions.push_back(FileWatcher::Watch(m_FragmentSourcePath, onFileChanged));

        m_RendererID = CreateShader(File::Read(m_VertexSourcePath), File::Read(m_FragmentSourcePath));

        if (m_RendererID == 0)
            HAM_CORE_ERROR("Shader compilation failed");
        else
            Shader::s_Shaders.push_back(this);
    }

    Shader::~Shader()
    {
        for (auto &unSubscribeFunction : m_UnSubscribeFunctions)
            unSubscribeFunction();

        Shader::s_Shaders.erase(std::find(Shader::s_Shaders.begin(), Shader::s_Shaders.end(), this));
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

    void Shader::Reload()
    {
        if (m_LastReloadTime + std::chrono::milliseconds(100) > std::chrono::system_clock::now())
            return;
        m_LastReloadTime = std::chrono::system_clock::now();

        m_ShouldReload = true;
    }

    void Shader::PerformReload()
    {
        if (!m_ShouldReload)
            return;
        m_ShouldReload = false;

        m_UniformLocationCache.clear();
        auto newProgram = CreateShader(File::Read(m_VertexSourcePath), File::Read(m_FragmentSourcePath));
        if (newProgram == 0)
        {
            HAM_CORE_ERROR("Shader recompilation failed:\n\t{0}\n\t{1}", m_VertexSourcePath, m_FragmentSourcePath);
            return;
        }
        glDeleteProgram(m_RendererID);
        m_RendererID = newProgram;
        // HAM_CORE_INFO("Reloaded shader:\n\t{0}\n\t{1}", m_VertexSourcePath, m_FragmentSourcePath);
    }

    void Shader::SetUniform1i(const std::string &name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniform2i(const std::string &name, glm::ivec2 value)
    {
        glUniform2i(GetUniformLocation(name), value.x, value.y);
    }

    void Shader::SetUniform3i(const std::string &name, glm::ivec3 value)
    {
        glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void Shader::SetUniform4i(const std::string &name, glm::ivec4 value)
    {
        glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniform1f(const std::string &name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void Shader::SetUniform2f(const std::string &name, glm::vec2 value)
    {
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }

    void Shader::SetUniform3f(const std::string &name, glm::vec3 value)
    {
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void Shader::SetUniform4f(const std::string &name, glm::vec4 value)
    {
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniformMat3f(const std::string &name, const glm::mat3 &matrix)
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
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
        int result = -1;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result != GL_TRUE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char message[1024];
            glGetShaderInfoLog(id, length, &length, message);

            std::string shaderType;
            switch (type)
            {
            // clang-format off
                case GL_VERTEX_SHADER: shaderType = "vertex"; break; 
                case GL_FRAGMENT_SHADER: shaderType = "fragment"; break;
                default: shaderType = "unknown"; break;
                // clang-format on
            }

            HAM_CORE_ERROR("Failed to compile {0} shader!", shaderType);
            HAM_CORE_ERROR("OpenGL Error: {0}", message);
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
    {
        auto program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        if (vs == 0 || fs == 0)
        {
            glDeleteShader(vs);
            glDeleteShader(fs);
            return program;
        }

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        // Error handling
        int result = -1;
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        if (result != GL_TRUE)
        {
            int length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            char *message = (char *)alloca(length * sizeof(char));
            glGetProgramInfoLog(program, length, &length, message);

            HAM_CORE_ERROR("Failed to link shader program!");
            HAM_CORE_ERROR("{0}", message);
            glDeleteShader(vs);
            glDeleteShader(fs);
            return 0;
        }

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
