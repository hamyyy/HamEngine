#pragma once

#include "Ham/Scene/Component.h"

namespace Ham {
enum class CameraMode {
  FPS,
  ORBIT
};

class CameraController : public NativeScript {
  using NativeScript::NativeScript;

 public:
  void OnCreate();
  void OnDestroy();
  void OnUpdate(TimeStep deltaTime);
  void OnUIUpdate(TimeStep deltaTime);

  void SetTarget(const math::vec3 &target, bool animate = true)
  {
    if (animate) {
      m_Animate = true;
      m_AnimationTarget = target;
    }
    else
      m_Target.Column(3) = math::vec4(target, m_Target.Column(3)(0, 3));
  }

  void CalculateAlphaBeta();

  float GetAlpha() const { return m_Alpha; }
  float GetBeta() const { return m_Beta; }
  float GetDistance() const { return m_Distance; }
  math::mat4 GetTarget() const { return m_Target; }

  void SetAlpha(float alpha) { m_Alpha = alpha; }
  void SetBeta(float beta) { m_Beta = beta; }
  void SetDistance(float distance) { m_Distance = distance; }
  void SetTarget(const math::mat4 &target) { m_Target = target; }

 private:
  void DoAnimation(TimeStep deltaTime);

 private:
  float m_Alpha;
  float m_Beta;
  float m_Distance;
  math::mat4 m_Target;
  math::vec3 m_AnimationTarget;
  bool m_Animate = false;

  CameraMode m_CameraMode = CameraMode::ORBIT;
};
}  // namespace Ham