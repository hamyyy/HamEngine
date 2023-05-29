#pragma once

#include "Ham/Scene/Scene.h"
#include "Ham/Scene/Component.h"
#include "Ham/Core/Base.h"

#include <entt/entt.hpp>

namespace Ham
{
    class Entity
    {
    public:
        Entity();
        Entity(entt::entity handle, Scene *scene);

        template <typename T, typename... Args>
        T &AddComponent(Args &&...args)
        {
            HAM_ASSERT(!HasComponent<T>(), "Entity already has component!");
            return m_Registry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template <typename T>
        T &GetComponent()
        {
            HAM_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_Registry->get<T>(m_EntityHandle);
        }

        template <typename T>
        bool HasComponent()
        {
            return m_Registry->try_get<T>(m_EntityHandle) != nullptr;
        }

        template <typename T>
        void RemoveComponent()
        {
            HAM_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_Registry->remove<T>(m_EntityHandle);
        }

        void Destroy()
        {
            HAM_ASSERT(*this, "Entity is null!");
            m_Registry->destroy(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null && m_Registry != nullptr; }
        bool operator==(const Entity &other) const { return m_EntityHandle == other.m_EntityHandle && m_Registry == other.m_Registry; }
        bool operator!=(const Entity &other) const { return !(*this == other); }

    private:
        entt::entity m_EntityHandle{entt::null};
        entt::registry *m_Registry;
    };
} // namespace Ham
