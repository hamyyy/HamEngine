#include "Ham/Scene/Scene.h"

#include "Ham/Scene/Component.h"
#include "Ham/Scene/Entity.h"

namespace Ham
{
    Scene::Scene() : m_Registry() {}

    Scene::~Scene() {}

    Entity Scene::CreateEntity(std::string name)
    {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<Component::ID>();
        entity.AddComponent<Component::Tag>(name);
        entity.AddComponent<Component::Transform>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        entity.Destroy();
    }

    Entity Scene::GetEntityByID(uint64_t id)
    {
        const auto &view = m_Registry.view<Component::ID>();

        for (auto entity : view)
        {
            if (view.get<Component::ID>(entity).Value == id)
            {
                return {entity, this};
            }
        }

        return {};
    }

    std::vector<Entity> Scene::GetEntitiesByTag(std::string name)
    {
        std::vector<Entity> entities;
        const auto &view = m_Registry.view<Component::Tag>();

        for (auto entity : view)
        {
            if (view.get<Component::Tag>(entity).Name == name)
            {
                entities.push_back({entity, this});
            }
        }

        return entities;
    }

    entt::registry *Scene::GetRegistry() { return &m_Registry; }

    Entity Scene::GetActiveCamera()
    {
        const auto &view = m_Registry.view<Component::Camera>();

        for (auto entity : view)
        {
            if (view.get<Component::Camera>(entity).Active)
                return {entity, this};
        }

        return {};
    }
} // namespace Ham
