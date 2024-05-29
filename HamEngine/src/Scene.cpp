#include "Ham/Scene/Scene.h"

#include "Ham/Scene/Component.h"
#include "Ham/Scene/Entity.h"
#include "Ham/Util/Watcher.h"

namespace Ham {
Scene::Scene() : m_Registry() {}

Scene::~Scene()
{
  m_Registry.clear();
}

void Scene::Init() {}

Entity Scene::CreateEntity(std::string name)
{
  static uint32_t order = 0;
  Entity entity = {m_Registry.create(), this};
  entity.AddComponent<Component::UUID>(UUIDGenerator::Create());
  entity.AddComponent<Component::Tag>(name).Order = order++;
  entity.AddComponent<Component::Transform>();
  entity.AddComponent<Component::EntityList>();
  entity.AddComponent<Component::ShaderList>();
  return entity;
}

void Scene::DestroyEntity(Entity entity)
{
  entity.Destroy();
}

Entity Scene::GetEntityByID(UUID id)
{
  const auto &view = m_Registry.view<Component::UUID>();

  for (auto entity : view) {
    if (view.get<Component::UUID>(entity) == id) {
      return {entity, this};
    }
  }

  return {};
}

std::vector<Entity> Scene::GetEntitiesByTag(std::string name)
{
  std::vector<Entity> entities;
  const auto &view = m_Registry.view<Component::Tag>();

  for (auto entity : view) {
    if (view.get<Component::Tag>(entity).Name == name) {
      entities.push_back({entity, this});
    }
  }

  return entities;
}

entt::registry *Scene::GetRegistry() { return &m_Registry; }

Entity Scene::GetActiveCamera()
{
  if (m_ActiveCamera != nullptr)
    return *m_ActiveCamera;

  const auto &view = m_Registry.view<Component::Camera>();

  for (auto entity : view) {
    if (view.get<Component::Camera>(entity).Active)
      return {entity, this};
  }

  return {};
}

void Scene::SetActiveCamera(Entity *cameraEntity)
{
  m_ActiveCamera = cameraEntity;
}

Entity &Scene::GetSelectedEntity()
{
  static Entity selectedEntity = {};
  return selectedEntity;
}

void Scene::SetSelectedEntity(Entity entity)
{
  GetSelectedEntity() = entity;
}

void Scene::ClearSelectedEntity()
{
  GetSelectedEntity() = {};
}

Entity &Scene::GetHoveredEntity()
{
  static Entity hoveredEntity = {};
  return hoveredEntity;
}

void Scene::SetHoveredEntity(Entity entity)
{
  GetHoveredEntity() = entity;
}

void Scene::ClearHoveredEntity()
{
  GetHoveredEntity() = {};
}

std::vector<Entity> Scene::GetEntities()
{
  std::vector<Entity> entities;
  const auto &view = m_Registry.view<Component::UUID>();

  for (auto entity : view) {
    entities.push_back({entity, this});
  }

  return entities;
}

std::vector<Entity> Scene::GetTopLevelEntities()
{
  std::vector<Entity> entities;
  const auto &view = m_Registry.view<Component::UUID>();

  for (auto entity : view) {
    Entity e = {entity, this};
    if (!e.HasComponent<Component::Parent>())
      entities.push_back(e);
  }

  return entities;
}
}  // namespace Ham
