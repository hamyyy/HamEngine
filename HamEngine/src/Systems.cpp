#include "Ham/Scene/Systems.h"

#include "Ham/Scene/Entity.h"
#include "Ham/Scene/Scene.h"
#include "Ham/Core/Base.h"

namespace Ham
{
    void Systems::AttachNativeScripts(Scene &scene)
    {
        auto view = scene.m_Registry.view<Component::NativeScriptList>();
        for (auto entity : view)
        {
            auto &list = view.get<Component::NativeScriptList>(entity);
            for (auto &script : list.Scripts)
            {
                if (!script->Active)
                {
                    if (script->World == nullptr)
                        script->World = &scene;
                    if (script->GameObject == nullptr || entity != entt::null)
                        script->GameObject = new Entity(entity, &scene);
                    script->Active = true;
                    script->OnCreate();
                }
            }
        }
    }

    void Systems::DetachNativeScripts(Scene &scene)
    {
        auto view = scene.m_Registry.view<Component::NativeScriptList>();
        for (auto entity : view)
        {
            auto &list = view.get<Component::NativeScriptList>(entity);
            for (auto &script : list.Scripts)
            {
                if (script->Active)
                {
                    if (script->World == nullptr)
                        script->World = &scene;
                    if (script->GameObject == nullptr || entity != entt::null)
                        script->GameObject = new Entity(entity, &scene);
                    script->OnDestroy();
                    script->Active = false;
                }
            }
        }
    }

    void Systems::UpdateNativeScripts(Scene &scene, TimeStep &deltaTime)
    {
        auto view = scene.m_Registry.view<Component::NativeScriptList>();
        for (auto entity : view)
        {
            auto &list = view.get<Component::NativeScriptList>(entity);
            for (auto &script : list.Scripts)
            {
                if (script->Active)
                {
                    if (script->World == nullptr)
                        script->World = &scene;
                    if (script->GameObject == nullptr || entity != entt::null)
                        script->GameObject = new Entity(entity, &scene);
                    script->OnUpdate(deltaTime);
                }
            }
        }
    }
} // namespace Ham
