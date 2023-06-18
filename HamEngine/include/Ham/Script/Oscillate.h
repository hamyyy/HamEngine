#pragma once

#include "Ham/Scene/Component.h"

namespace Ham
{

    class Oscillate : public NativeScript
    {
        using NativeScript::NativeScript;

    public:
        void OnCreate();
        void OnDestroy();
        void OnUpdate(TimeStep deltaTime);
        void OnUIUpdate(TimeStep deltaTime);
    };
}