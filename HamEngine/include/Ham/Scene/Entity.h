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
        Entity(entt::entity handle, entt::registry *registry);
        Entity(const Entity &other) = default;

        template <typename T, typename... Args>
        T &AddComponent(Args &&...args)
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            if (HasComponent<T>())
            {
                HAM_CORE_ERROR("Entity already has component!");
                return GetComponent<T>();
            }
            return m_Registry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template <typename T>
        T &GetComponent()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            HAM_CORE_ASSERT(HasComponent<T>(), std::string("Entity does not have \"") + typeid(T).name() + "\"");
            return m_Registry->get<T>(m_EntityHandle);
        }

        template <typename T>
        bool HasComponent()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            return m_Registry->try_get<T>(m_EntityHandle) != nullptr;
        }

        template <typename T>
        void RemoveComponent()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            HAM_CORE_ASSERT(HasComponent<T>(), std::string("Entity does not have \"") + typeid(T).name() + "\"");
            m_Registry->remove<T>(m_EntityHandle);
        }

        void Destroy()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            m_Registry->destroy(m_EntityHandle);
        }

        UUID GetUUID()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            return GetComponent<Component::UUID>();
        }

        std::string GetName()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            return GetComponent<Component::Tag>().Name;
        }

        bool HasParent()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            return HasComponent<Component::Parent>();
        }

        Entity GetParent()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            if (HasParent())
                return GetComponent<Component::Parent>();

            return {};
        }

        void SetParent(Entity parent)
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            
            if (!parent)
                return;

            parent.GetComponent<Component::EntityList>().Add({m_EntityHandle, m_Registry});
            if (HasParent())
                RemoveComponent<Component::Parent>();
            AddComponent<Component::Parent>(parent);
        }

        void RemoveParent()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            if (HasParent())
            {
                Entity parent = GetParent();
                parent.GetComponent<Component::EntityList>().Remove(*this);
                RemoveComponent<Component::Parent>();
            }
        }

        std::vector<Entity> GetChildren()
        {
            HAM_CORE_ASSERT(m_EntityHandle != entt::null, "Entity is null!");
            return GetComponent<Component::EntityList>().Entities;
        }

        operator bool() const { return m_EntityHandle != entt::null && m_Registry != nullptr; }
        bool operator==(const Entity &other) const { return m_EntityHandle == other.m_EntityHandle && m_Registry == other.m_Registry; }
        bool operator!=(const Entity &other) const { return !(*this == other); }

        entt::entity GetHandle() const { return m_EntityHandle; }

    protected:
        entt::entity m_EntityHandle{entt::null};
        entt::registry *m_Registry;
    };
} // namespace Ham
