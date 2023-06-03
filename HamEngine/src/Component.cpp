#include "Ham/Scene/Component.h"

#include "Ham/Scene/Scene.h"
#include "Ham/Scene/Entity.h"

namespace Ham
{
    NativeScript::NativeScript(const std::string &name) : Name(name), ID(UUIDGenerator::Create()) {}
    NativeScript::NativeScript(const NativeScript &other) : Name(other.Name), ID(other.ID) {}
} // namespace Ham::Component
