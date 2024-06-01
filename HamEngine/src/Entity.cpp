#include "Ham/Scene/Entity.h"

namespace Ham {
Entity::Entity() : m_EntityHandle(entt::null), m_Registry(nullptr) {}
Entity::Entity(entt::entity handle, Scene *scene) : m_EntityHandle(handle), m_Registry(scene->GetRegistry()) {}
Entity::Entity(entt::entity handle, entt::registry *registry) : m_EntityHandle(handle), m_Registry(registry) {}
}  // namespace Ham