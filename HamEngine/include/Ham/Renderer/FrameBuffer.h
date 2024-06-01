#pragma once

#include "Ham/Core/Math.h"

#include "glad/gl.h"

#include <cstdint>
#include <map>
#include <vector>

namespace Ham {

enum AttachmentType {
  COLOR = GL_COLOR_BUFFER_BIT,
  DEPTH = GL_DEPTH_BUFFER_BIT,
  STENCIL = GL_STENCIL_BUFFER_BIT,
};

enum class TextureFormat {
  NONE = 0,

  COLOR_RI = GL_R8I,
  COLOR_RGI = GL_RG8I,
  COLOR_RGBI = GL_RGB8I,
  COLOR_RGBAI = GL_RGBA8I,

  COLOR_RUI = GL_R8UI,
  COLOR_RGUI = GL_RG8UI,
  COLOR_RGBUI = GL_RGB8UI,
  COLOR_RGBAUI = GL_RGBA8UI,

  COLOR_R8 = GL_R8,
  COLOR_RG8 = GL_RG8,
  COLOR_RGB8 = GL_RGB8,
  COLOR_RGBA8 = GL_RGBA8,

  COLOR_R16 = GL_R16,
  COLOR_RG16 = GL_RG16,
  COLOR_RGB16 = GL_RGB16,
  COLOR_RGBA16 = GL_RGBA16,

  COLOR_R32F = GL_R32F,
  COLOR_RG32F = GL_RG32F,
  COLOR_RGB32F = GL_RGB32F,
  COLOR_RGBA32F = GL_RGBA32F,

  DEPTH24 = GL_DEPTH_COMPONENT24,
  STENCIL8 = GL_STENCIL_INDEX8,

  DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
};

enum TextureFilter {
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR,
};

struct FrameBufferSpecification {
  uint32_t Width, Height;
  math::vec4 ClearColor = {0.0f, 0.0f, 0.0f, 1.0f};

  std::vector<TextureFormat> ColorAttachments = {TextureFormat::COLOR_RGBA8};
  TextureFormat DepthAttachment = TextureFormat::DEPTH24_STENCIL8;

  uint32_t Samples = 1;

  TextureFilter MinFilter = TextureFilter::LINEAR;
  TextureFilter MagFilter = TextureFilter::LINEAR;
};

class FrameBuffer {
 public:
  FrameBuffer();
  FrameBuffer(const FrameBufferSpecification &spec);
  ~FrameBuffer();

  void Init(const FrameBufferSpecification &spec);

  void Bind() const;
  void Unbind() const;

  void Resize(uint32_t width, uint32_t height);

  void Clear(uint32_t attachmentType) const;

  void SetClearColor(const math::vec4 &color);
  const math::vec4 &GetClearColor() const;

  void SetColorAttachment(uint32_t index, TextureFormat format);
  void SetDepthAttachment(TextureFormat format);

  const TextureFormat &GetColorAttachment(uint32_t index) const;
  const TextureFormat &GetDepthAttachment() const;

  uint32_t GetColorAttachmentID(uint32_t index) const;
  uint32_t GetDepthAttachmentID() const;

  uint32_t GetWidth() const;
  uint32_t GetHeight() const;

  void Invalidate();

  std::vector<unsigned char> ReadPixels() const;
  std::vector<unsigned char> ReadPixels(int x, int y, int width, int height) const;

  const FrameBufferSpecification &GetSpecification() const;

 private:
  FrameBufferSpecification m_Specification;

  uint32_t m_RendererID = 0;
  std::vector<uint32_t> m_ColorAttachments;
  uint32_t m_DepthAttachment = 0;
};
}  // namespace Ham
