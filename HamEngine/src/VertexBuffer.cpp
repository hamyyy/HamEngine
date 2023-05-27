#include "Ham/Renderer/VertexBuffer.h"

#include <glad/glad.h>

namespace Ham
{
    VertexBuffer::VertexBuffer()
    {
        glCreateBuffers(1, &m_RendererID);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::SetData(const void *data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer *VertexBuffer::Create()
    {
        return new VertexBuffer();
    }
} // namespace Ham
