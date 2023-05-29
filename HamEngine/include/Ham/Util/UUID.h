#pragma once

#define UUID_SYSTEM_GENERATOR
#include <uuid.h>

namespace Ham
{
    class UUID
    {
    public:
        UUID() : m_UUID(uuid::uuid_system_generator{}) {}

        std::string ToString() const { return m_UUID.to_string(); }

    private:
        uuid m_UUID;
    };
} // namespace Ham
