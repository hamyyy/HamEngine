#include "Ham/Script/Oscillate.h"

#include "Ham/Core/Application.h"
#include "Ham/Input/Input.h"
#include "Ham/Scene/Entity.h"

#include "Ham/Util/GlmExtra.h"

namespace Ham
{
    void Oscillate::OnCreate()
    {
    }

    void Oscillate::OnDestroy()
    {
    }

    void Oscillate::OnUpdate(TimeStep deltaTime)
    {
        float time = Application::Get().GetTime();

        auto &Position = GameObject->GetComponent<Component::Transform>().Position;

        float x = Position.x;
        float y = Position.y;
        Position.z = 1.0f * sin(0.15f * x + time) * cos(0.15f * y + time);
    }

    void Oscillate::OnUIUpdate(TimeStep deltaTime)
    {
    }

} // namespace Ham