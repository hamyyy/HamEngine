#include "Ham/Script/CameraController.h"

#include "Ham/Input/Input.h"
#include "Ham/Scene/Entity.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Ham/Util/GlmExtra.h"

namespace Ham
{
    void CameraController::OnCreate()
    {
        m_Target = glm::inverse(glm::camera());
    }

    void CameraController::OnDestroy()
    {
    }

    void CameraController::OnUpdate(TimeStep deltaTime)
    {
        DoAnimation(deltaTime);

        static auto pTarget = glm::inverse(glm::camera());
        static auto pAlpha = 0.0f;
        static auto pBeta = 0.0f;
        static auto pDistance = 0.0f;

        static float speed = 5.0f;
        static auto pmousePos = Input::GetMousePosition();
        auto mousePos = Input::GetMousePosition();

        auto &transform = GameObject->GetComponent<Component::Transform>();

        glm::vec3 forwardTarget = m_Target * glm::vec4(glm::forward(), 0.0f);
        glm::vec3 rightTarget = m_Target * glm::vec4(glm::right(), 0.0f);
        glm::vec3 upTarget = m_Target * glm::vec4(glm::up(), 0.0f);

        glm::vec3 cameraForward = glm::inverse(m_Target) * transform.ToMatrix() * glm::camera() * glm::vec4(glm::forward(), 0.0f);
        glm::vec3 cameraRight = glm::inverse(m_Target) * transform.ToMatrix() * glm::camera() * glm::vec4(glm::right(), 0.0f);
        glm::vec3 cameraUp = glm::inverse(m_Target) * transform.ToMatrix() * glm::camera() * glm::vec4(glm::up(), 0.0f);

        glm::vec3 strightUp = glm::inverse(m_Target) * glm::vec4(glm::up(), 0.0f);

#if defined(Y_UP)
        float flip = glm::forward().z;
#else
        float flip = glm::forward().y;
#endif

        if (Input::IsKeyDown(KeyCode::W))
        {
            m_Target *= glm::translate(glm::mat4(1.0f), cameraForward * speed * (float)deltaTime);
        }

        if (Input::IsKeyDown(KeyCode::S))
        {
            m_Target *= glm::translate(glm::mat4(1.0f), -cameraForward * speed * (float)deltaTime);
        }

        if (Input::IsKeyDown(KeyCode::D))
        {
            m_Target *= glm::translate(glm::mat4(1.0f), cameraRight * speed * (float)deltaTime);
        }

        if (Input::IsKeyDown(KeyCode::A))
        {
            m_Target *= glm::translate(glm::mat4(1.0f), -cameraRight * speed * (float)deltaTime);
        }

        if (Input::IsKeyDown(KeyCode::SPACE))
        {
            m_Target *= glm::translate(glm::mat4(1.0f), strightUp * speed * (float)deltaTime);
        }

        if (Input::IsKeyDown(KeyCode::LEFT_CONTROL))
        {
            m_Target *= glm::translate(glm::mat4(1.0f), -strightUp * speed * (float)deltaTime);
        }

        // zoom camera
        auto wheelDelta = Input::GetMouseWheelDelta();
        if (wheelDelta != 0)
        {
            m_Distance += wheelDelta * (m_Distance * 0.1f);
            m_Distance = glm::max(m_Distance, 0.1f);
            m_Distance = glm::min(m_Distance, 1000.0f);
        }

        // translate camera
        if (Input::IsMouseButtonDown(MouseButton::MIDDLE))
        {
            glm::vec2 mouseDelta = mousePos - pmousePos;
            m_Target *= glm::translate(glm::mat4(1.0f), -cameraRight * mouseDelta.x * m_Distance * 0.00083f);
            m_Target *= glm::translate(glm::mat4(1.0f), cameraUp * mouseDelta.y * m_Distance * 0.00083f);
        }

        // rotate camera
        if (Input::IsMouseButtonDown(MouseButton::RIGHT))
        {
            glm::vec2 mouseDelta = mousePos - pmousePos;

            m_Alpha += mouseDelta.x * flip * glm::radians(0.1f);
            m_Beta -= mouseDelta.y * glm::radians(0.1f);
        }

        // clamp beta
        m_Beta = glm::clamp(m_Beta, glm::radians(-89.0f), glm::radians(89.0f));

        // calculate new transform
        glm::mat4 newTransform = m_Target                                            //
                                 * glm::rotate(glm::mat4(1.0f), m_Alpha, upTarget)   //
                                 * glm::rotate(glm::mat4(1.0f), m_Beta, rightTarget) //
                                 * glm::translate(glm::mat4(1.0f), flip * forwardTarget * (m_Distance));

        // float x = m_Distance * glm::cos(m_Beta) * glm::sin(m_Alpha);
        // float y = m_Distance * glm::sin(m_Beta);
        // float z = m_Distance * glm::cos(m_Beta) * glm::cos(m_Alpha);
        // m_Position = m_Target + glm::vec3(x, y, z);

        transform = newTransform;

        pmousePos = mousePos;
        pTarget = m_Target;
        pAlpha = m_Alpha;
        pBeta = m_Beta;
        pDistance = m_Distance;
    }

    void CameraController::DoAnimation(TimeStep deltaTime)
    {
        if (!m_Animate)
            return;

        if (glm::distance(glm::vec3(m_Target[3]), m_AnimationTarget) <= 0.01f)
        {
            m_Animate = false;
            m_Target[3] = glm::vec4(m_AnimationTarget, m_Target[3].w);
            return;
        }

        auto anim = m_Target;
        anim[3] = glm::vec4(m_AnimationTarget, anim[3].w);
        m_Target = glm::mix(m_Target, anim, 30.0f * deltaTime);
    }

} // namespace Ham