#include "Ham/Scene/Component.h"

#include "Ham/Scene/Scene.h"
#include "Ham/Scene/Entity.h"

namespace Ham {
NativeScript::NativeScript(const std::string &name) : Name(name), ID(UUIDGenerator::Create()) {}
NativeScript::NativeScript(const NativeScript &other) : Name(other.Name), ID(other.ID) {}

namespace Component {

void EntityList::Add(Entity entity)
{
  Entities.push_back(entity);
}

void EntityList::Remove(Entity entity)
{
  auto uuid = entity.GetUUID();
  for (auto it = Entities.begin(); it != Entities.end(); it++) {
    if (it->GetUUID() == uuid) {
      Entities.erase(it);
      return;
    }
  }
}

bool EntityList::Contains(Entity entity)
{
  auto uuid = entity.GetUUID();
  for (auto it = Entities.begin(); it != Entities.end(); it++) {
    if (it->GetUUID() == uuid) {
      return true;
    }
  }
  return false;
}

}  // namespace Component
}  // namespace Ham
