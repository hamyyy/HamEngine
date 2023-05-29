#pragma once

#include "Ham/Util/UUID.h"

#include <entt/entt.hpp>

namespace Ham
{
    class Entity;
    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(std::string name);
        void DestroyEntity(Entity entity);
        Entity GetEntityByID(UUID id);
        Entity GetActiveCamera();
        std::vector<Entity> GetEntitiesByTag(std::string name);
        entt::registry *GetRegistry();

    private:
        entt::registry m_Registry;
        friend class Entity;
    };
} // namespace Ham