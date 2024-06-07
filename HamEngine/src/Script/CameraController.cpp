#include "Ham/Script/CameraController.h"

#include "Ham/Core/Application.h"
#include "Ham/Core/Math.h"
#include "Ham/Input/Input.h"
#include "Ham/Scene/Entity.h"
#include "Ham/Events/Event.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Ham {
void CameraController::OnCreate()
{
  HAM_CORE_INFO("CameraController::OnCreate()");
  auto &transformComponent = GameObject->GetComponent<Component::Transform>();
  auto transform = transformComponent.ToMatrix();

  HAM_CORE_INFO("Camera transfotm: {0}", transform);

  m_Distance = 10.0f;

  m_Target = transform * math::translate(m_Distance * math::forward());
  m_Target.Submatrix<3, 3>(0, 0) = math::inverse(math::camera()).Submatrix<3, 3>(0, 0);

  CalculateAlphaBeta();

  m_AnimationTarget = math::zero<math::vec3>();
}

void CameraController::CalculateAlphaBeta()
{
  // given a target and a camera position, calculate alpha and beta
  auto &transformComponent = GameObject->GetComponent<Component::Transform>();
  auto transform = transformComponent.ToMatrix();
  m_Target = math::inverse(math::camera());

  auto targetPos = math::vec3(m_Target.Submatrix<3, 1>(0, 3));
  auto cameraPos = math::vec3(transform.Submatrix<3, 1>(0, 3));

  auto dir = cameraPos - targetPos;

  if (math::length(dir) < 0.01f) {
    dir = math::forward();
  }
  m_Distance = math::length(dir);

  dir = math::normalize(dir);

  math::vec3 cameraForward = (math::inverse(math::camera()) * math::vec4(math::forward(), 0.0f)).xyz;
  math::vec3 cameraRight = (math::inverse(math::camera()) * math::vec4(math::right(), 0.0f)).xyz;
  math::vec3 cameraUp = (math::inverse(math::camera()) * math::vec4(math::up(), 0.0f)).xyz;

  m_Alpha = math::pi<float> - std::atan2(math::length(dir * cameraForward), math::length(dir * cameraRight));
  m_Beta = -std::acos(math::length(dir * cameraUp));
  m_Beta = math::clamp(m_Beta, -math::pi<float> / 2.0f + 0.01f, math::pi<float> / 2.0f - 0.01f);

  HAM_INFO("Alpha: {0}, Beta: {1}", m_Alpha, m_Beta);
}

void CameraController::OnDestroy()
{
}

void CameraController::OnUIUpdate(TimeStep deltaTime)
{
  auto &transformComponent = GameObject->GetComponent<Component::Transform>();
  auto transform = transformComponent.ToMatrix();

  auto targetPos = math::vec3(m_Target.Submatrix<3, 1>(0, 3));
  auto cameraPos = math::vec3(transform.Submatrix<3, 1>(0, 3));

  auto dir = cameraPos - targetPos;
  if (math::length(dir) < 0.01f) {
    dir = math::forward();
  }
  dir = math::normalize(dir);

  math::vec2 mousePos = Input::GetMousePosition();
  static math::vec2 lastMousePos = mousePos;
  auto mouseDelta = mousePos - lastMousePos;

  if (Input::IsMouseButtonDown(Button::BUTTON_0)) {
    m_Alpha -= mouseDelta.x * 0.007f;
    m_Beta -= mouseDelta.y * 0.007f;
    m_Beta = math::clamp(m_Beta, -math::pi<float> / 2.0f + 0.01f, math::pi<float> / 2.0f - 0.01f);
  }

  // dispatch a mousebutton event if the mouse is down
  if (Input::IsMouseButtonDownThisFrame(MouseButton::LEFT)) {
    Events::Emit<Events::MouseMoved>(Input::GetMousePosition().x, Input::GetMousePosition().y);
    Events::Emit<Events::MouseButtonPressed>(MouseButton::LEFT);
  }

  auto wheelDelta = Input::GetMouseWheelDelta();
  if (wheelDelta != 0) {
    m_Distance -= wheelDelta * std::min(m_Distance * m_Distance, 500.0f) * 0.01f;
  }

  math::vec3 cameraForward = (math::camera() * math::vec4(math::forward(), 0.0f)).xyz;
  math::vec3 cameraRight = (math::camera() * math::vec4(math::right(), 0.0f)).xyz;
  math::vec3 cameraUp = (math::camera() * math::vec4(math::up(), 0.0f)).xyz;

  targetPos = math::vec3(m_Target.Submatrix<3, 1>(0, 3));

  auto newTf = math::inverse(math::camera()) * math::translate(targetPos) * math::rotateAxisAngle(cameraUp, m_Alpha) * math::rotateAxisAngle(cameraRight, m_Beta) * math::translate(-m_Distance * cameraForward);

  transformComponent = Component::Transform::FromMatrix(newTf);

  lastMousePos = mousePos;
}

void CameraController::OnUpdate(TimeStep deltaTime)
{
}

void CameraController::DoAnimation(TimeStep deltaTime)
{
}

}  // namespace Ham