#pragma once

#include "Ham/Core/Math.h"
#include "Ham/Util/File.h"

#include <atomic>
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace Ham {
class Application;
class Shader {
 public:
  Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath = "");
  ~Shader();

  void Bind() const;
  void Unbind() const;
  void Reload();
  void PerformReload();

  void SetUniform1i(const std::string &name, int value);
  void SetUniform2i(const std::string &name, math::ivec2 value);
  void SetUniform3i(const std::string &name, math::ivec3 value);
  void SetUniform4i(const std::string &name, math::ivec4 value);

  void SetUniform1f(const std::string &name, float value);
  void SetUniform2f(const std::string &name, math::vec2 value);
  void SetUniform3f(const std::string &name, math::vec3 value);
  void SetUniform4f(const std::string &name, math::vec4 value);

  void SetUniformMat3f(const std::string &name, const math::mat3 &matrix);
  void SetUniformMat4f(const std::string &name, const math::mat4 &matrix);

 private:
  unsigned int m_RendererID = 0;
  std::unordered_map<std::string, int> m_UniformLocationCache;

  unsigned int CompileShader(unsigned int type, const std::string &source);
  unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader, const std::string &geometryShader = "");
  int GetUniformLocation(const std::string &name);

  std::string m_VertexSourcePath;
  std::string m_FragmentSourcePath;
  std::string m_GeometrySourcePath;

  std::chrono::time_point<std::chrono::system_clock> m_LastReloadTime;
  std::atomic_bool m_ShouldReload = false;

  static std::vector<Shader *> s_Shaders;

  friend class Application;
};
}  // namespace Ham
