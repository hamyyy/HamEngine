#pragma once

#include "Ham/Util/UUID.h"

#include <entt/entt.hpp>

namespace Ham
{
    class Entity;
    class Systems;
    class Scene
    {
    public:
        Scene();
        ~Scene();

        void Init();
        void Shutdown();

        Scene &operator=(const Scene &other) { return *this; }

        Entity CreateEntity(std::string name);
        void DestroyEntity(Entity entity);
        Entity GetEntityByID(UUID id);

        Entity GetActiveCamera();

        Entity &GetSelectedEntity(); // TODO: Move entity selection to editor
        void SetSelectedEntity(Entity entity);
        void ClearSelectedEntity();

        Entity &GetHoveredEntity();
        void SetHoveredEntity(Entity entity);
        void ClearHoveredEntity();

        std::vector<Entity> GetEntitiesByTag(std::string name);
        entt::registry *GetRegistry();
        std::vector<Entity> GetEntities();
        std::vector<Entity> GetTopLevelEntities();

    private:
        entt::registry m_Registry;

        friend class Entity;
        friend class Systems;
        friend class Application;
    };
} // namespace Ham
