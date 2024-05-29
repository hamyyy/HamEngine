#include "Ham/Renderer/FrameBuffer.h"

#include "Ham/Core/Base.h"

#include "glad/gl.h"

namespace Ham
{

    FrameBuffer::FrameBuffer() {}

    FrameBuffer::FrameBuffer(const FrameBufferSpecification &spec)
        : m_Specification(spec)
    {
        Invalidate();
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void FrameBuffer::Init(const FrameBufferSpecification &spec)
    {
        m_Specification = spec;
        Invalidate();
    }

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void FrameBuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }

    void FrameBuffer::Clear(uint32_t attachmentType) const
    {
        glClearColor(m_Specification.ClearColor.x, m_Specification.ClearColor.y, m_Specification.ClearColor.z, m_Specification.ClearColor.w);
        glClear(attachmentType);
    }

    void FrameBuffer::SetClearColor(const math::vec4 &color)
    {
        m_Specification.ClearColor = color;
    }

    const math::vec4 &FrameBuffer::GetClearColor() const
    {
        return m_Specification.ClearColor;
    }

    void FrameBuffer::SetColorAttachment(uint32_t index, TextureFormat format)
    {
        HAM_ASSERT(index < m_Specification.ColorAttachments.size(), "Index out of range!");
        m_Specification.ColorAttachments[index] = format;
        Invalidate();
    }

    void FrameBuffer::SetDepthAttachment(TextureFormat format)
    {
        m_Specification.DepthAttachment = format;
        Invalidate();
    }

    const TextureFormat &FrameBuffer::GetColorAttachment(uint32_t index) const
    {
        HAM_ASSERT(index < m_Specification.ColorAttachments.size(), "Index out of range!");
        return m_Specification.ColorAttachments[index];
    }

    const TextureFormat &FrameBuffer::GetDepthAttachment() const
    {
        return m_Specification.DepthAttachment;
    }

    uint32_t FrameBuffer::GetColorAttachmentID(uint32_t index) const
    {
        HAM_ASSERT(index < m_ColorAttachments.size(), "Index out of range!");
        return m_ColorAttachments[index];
    }

    uint32_t FrameBuffer::GetDepthAttachmentID() const
    {
        return m_DepthAttachment;
    }

    uint32_t FrameBuffer::GetWidth() const
    {
        return m_Specification.Width;
    }

    uint32_t FrameBuffer::GetHeight() const
    {
        return m_Specification.Height;
    }

    const FrameBufferSpecification &FrameBuffer::GetSpecification() const
    {
        return m_Specification;
    }

    void FrameBuffer::Invalidate()
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);
            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }

        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        bool multisample = m_Specification.Samples > 1;

        for (uint32_t i = 0; i < m_Specification.ColorAttachments.size(); i++)
        {
            m_ColorAttachments.emplace_back();
            glGenTextures(1, &m_ColorAttachments.back());
            glBindTexture(GL_TEXTURE_2D, m_ColorAttachments.back());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorAttachments.back(), 0);

            GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE)
            {
                HAM_ERROR("Framebuffer is incomplete!");
            }
        }

        if (m_Specification.DepthAttachment != TextureFormat::NONE)
        {
            glGenTextures(1, &m_DepthAttachment);
            glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

            GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE)
            {
                HAM_ERROR("Framebuffer is incomplete!");
            }
        }

        if (m_Specification.ColorAttachments.size() > 1)
        {
            HAM_ASSERT(m_Specification.ColorAttachments.size() <= 4, "Only 4 color attachments are supported!");
            GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
            glDrawBuffers(m_Specification.ColorAttachments.size(), buffers);
        }
        else if (m_Specification.ColorAttachments.empty())
        {
            glDrawBuffer(GL_NONE);
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            HAM_ERROR("Framebuffer is incomplete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    std::vector<unsigned char> FrameBuffer::ReadPixels() const
    {
        std::vector<unsigned char> pixels(m_Specification.Width * m_Specification.Height * 4);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glReadPixels(0, 0, m_Specification.Width, m_Specification.Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return pixels;
    }

    std::vector<unsigned char> FrameBuffer::ReadPixels(int x, int y, int width, int height) const
    {
        std::vector<unsigned char> pixels(width * height * 4);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return pixels;
    }
} // namespace Ham
