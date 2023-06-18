#pragma once
#include "Ham.h"

#include "Ham/Scene/Component.h"

namespace Ham
{
    class Systems
    {
    public:
        static void AttachNativeScripts(Scene &scene);
        static void DetachNativeScripts(Scene &scene);
        static void UpdateNativeScripts(Scene &scene, TimeStep &deltaTime);
        static void UpdateNativeScriptsUI(Scene &scene, TimeStep &deltaTime);
        static void RenderScene(Application &app, Scene &scene, TimeStep &deltaTime);
        static void RenderObjectPickerFrame(Application &app, Scene &scene, TimeStep &deltaTime);
    };
} // namespace Ham
