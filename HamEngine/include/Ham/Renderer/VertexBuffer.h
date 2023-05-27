#pragma once

#include <cstdint>

namespace Ham
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void *data, uint32_t size);

        static VertexBuffer *Create();

    private:
        uint32_t m_RendererID;
    };
} // namespace Ham
