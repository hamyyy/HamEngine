#pragma once

#include <cstdint>

namespace Ham
{
    class Buffer
    {
    public:
        Buffer();
        ~Buffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void *data, uint32_t size);

        static Buffer *Create();

    private:
        uint32_t m_RendererID;
    };
} // namespace Ham
