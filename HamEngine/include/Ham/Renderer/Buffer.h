#pragma once

#include <glad/gl.h>

#include <cstdint>
#include <vector>

namespace Ham {

enum DrawMode {
  STATIC = GL_STATIC_DRAW,
  DYNAMIC = GL_DYNAMIC_DRAW,
  STREAM = GL_STREAM_DRAW
};

template <typename T, uint32_t BufferType>
class Buffer {
 public:
  Buffer() {}
  ~Buffer() {}

  void Create()
  {
    glGenBuffers(1, &m_BufferID);
    m_isInitialized = true;
  }

  void Destroy()
  {
    glDeleteBuffers(1, &m_BufferID);
    m_isInitialized = false;
  }

  void Bind()
  {
    glBindBuffer(BufferType, m_BufferID);
  }
  void Unbind() { glBindBuffer(BufferType, 0); }

  void SetData(std::vector<T> data)
  {
    m_Data = data;
    Bind();
    glBufferData(BufferType, (int)m_Data.size() * sizeof(T), m_Data.data(), m_DrawMode);
  }

  bool IsInitialized() { return m_isInitialized; }

  size_t Size() { return m_Data.size(); }

  std::vector<T> &GetData() { return m_Data; }

  template <typename U>
  void DefineAttribute(size_t offset, uint32_t length, uint32_t type, bool normalized = false)
  {
    glEnableVertexAttribArray(m_AttributeIndex);
    glVertexAttribPointer(m_AttributeIndex, length, type, normalized ? GL_TRUE : GL_FALSE, sizeof(T), (void *)offset);
    m_AttributeIndex++;
  }

  void SetDrawMode(uint32_t mode) { m_DrawMode = mode; }

  void DefineAttribute1f(size_t offset, bool normalized = false) { DefineAttribute<float>(offset, 1, GL_FLOAT, normalized); }
  void DefineAttribute2f(size_t offset, bool normalized = false) { DefineAttribute<math::vec2>(offset, 2, GL_FLOAT, normalized); }
  void DefineAttribute3f(size_t offset, bool normalized = false) { DefineAttribute<math::vec3>(offset, 3, GL_FLOAT, normalized); }
  void DefineAttribute4f(size_t offset, bool normalized = false) { DefineAttribute<math::vec4>(offset, 4, GL_FLOAT, normalized); }

  void DefineAttribute1i(size_t offset, bool normalized = false) { DefineAttribute<int>(offset, 1, GL_INT, normalized); }
  void DefineAttribute2i(size_t offset, bool normalized = false) { DefineAttribute<math::ivec2>(offset, 2, GL_INT, normalized); }
  void DefineAttribute3i(size_t offset, bool normalized = false) { DefineAttribute<math::ivec3>(offset, 3, GL_INT, normalized); }
  void DefineAttribute4i(size_t offset, bool normalized = false) { DefineAttribute<math::ivec4>(offset, 4, GL_INT, normalized); }

  void DefineAttribute1ui(size_t offset, bool normalized = false) { DefineAttribute<uint32_t>(offset, 1, GL_UNSIGNED_INT, normalized); }
  void DefineAttribute2ui(size_t offset, bool normalized = false) { DefineAttribute<math::uvec2>(offset, 2, GL_UNSIGNED_INT, normalized); }
  void DefineAttribute3ui(size_t offset, bool normalized = false) { DefineAttribute<math::uvec3>(offset, 3, GL_UNSIGNED_INT, normalized); }
  void DefineAttribute4ui(size_t offset, bool normalized = false) { DefineAttribute<math::uvec4>(offset, 4, GL_UNSIGNED_INT, normalized); }

  void DefineAttributeMat2(size_t offset, bool normalized = false) { DefineAttribute<math::mat2>(offset, 2 * 2, GL_FLOAT, normalized); }
  void DefineAttributeMat3(size_t offset, bool normalized = false) { DefineAttribute<math::mat3>(offset, 3 * 3, GL_FLOAT, normalized); }
  void DefineAttributeMat4(size_t offset, bool normalized = false) { DefineAttribute<math::mat4>(offset, 4 * 4, GL_FLOAT, normalized); }

  void DefineAttribute1b(size_t offset, bool normalized = false) { DefineAttribute<bool>(offset, 1, GL_BOOL, normalized); }
  void DefineAttribute2b(size_t offset, bool normalized = false) { DefineAttribute<math::bvec2>(offset, 2, GL_BOOL, normalized); }
  void DefineAttribute3b(size_t offset, bool normalized = false) { DefineAttribute<math::bvec3>(offset, 3, GL_BOOL, normalized); }
  void DefineAttribute4b(size_t offset, bool normalized = false) { DefineAttribute<math::bvec4>(offset, 4, GL_BOOL, normalized); }

 private:
  uint32_t m_BufferID;
  DrawMode m_DrawMode = DrawMode::STATIC;

  std::vector<T> m_Data;

  uint32_t m_AttributeIndex = 0;

  bool m_isInitialized = false;
};

template <typename T>
using VertexBuffer = Buffer<T, GL_ARRAY_BUFFER>;
using IndexBuffer = Buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER>;

class VertexArray {
 public:
  VertexArray() {}
  ~VertexArray() { Destroy(); }

  void Create()
  {
    glGenVertexArrays(1, &m_VertexArrayID);
    m_isInitialized = true;
  }
  void Destroy()
  {
    glDeleteVertexArrays(1, &m_VertexArrayID);
    m_isInitialized = false;
  }

  void Bind() { glBindVertexArray(m_VertexArrayID); }
  void Unbind() { glBindVertexArray(0); }

  bool IsInitialized() { return m_isInitialized; }

 private:
  uint32_t m_VertexArrayID;

  bool m_isInitialized = false;
};

}  // namespace Ham
