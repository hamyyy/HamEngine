#pragma once

#include "Ham/Scene/Component.h"

namespace Ham
{
    enum class CameraMode
    {
        FPS,
        ORBIT
    };

    class CameraController : public NativeScript
    {
        using NativeScript::NativeScript;

    public:
        void OnCreate();
        void OnDestroy();
        void OnUpdate(TimeStep deltaTime);

        void SetTarget(const glm::vec3 &target, bool animate = true)
        {
            if (animate)
            {
                m_Animate = true;
                m_AnimationTarget = target;
            }
            else
                m_Target[3] = glm::vec4(target, m_Target[3].w);
        }

    private:
        void DoAnimation(TimeStep deltaTime);

    private:
        float m_Alpha = 0.0f;
        float m_Beta = 0.0f;
        float m_Distance = 10.0f;
        glm::mat4 m_Target = glm::mat4(1.0f);
        glm::vec3 m_AnimationTarget = glm::vec3(0.0f);
        bool m_Animate = false;
        
        CameraMode m_CameraMode = CameraMode::ORBIT;
    };
}