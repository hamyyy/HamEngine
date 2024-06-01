#pragma once

#include "Ham/Core/Base.h"
#include "Ham/Util/TimeStep.h"
// #include "Hazel/Events/Event.h"

namespace Ham {
class Layer {
 public:
  Layer(const std::string &name = "Layer") : m_DebugName(name) {}
  virtual ~Layer() = default;

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(TimeStep deltaTime) {}
  virtual void OnUIRender(TimeStep deltaTime) {}
  // virtual void OnEvent(Event &event) {}

  const std::string &GetName() const { return m_DebugName; }

 protected:
  std::string m_DebugName;
};

}  // namespace Ham