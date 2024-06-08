#pragma once

#include "Ham.h"

namespace Ham {
class EditorLayer : public Layer {
 public:
  EditorLayer(Application *app);
  ~EditorLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUpdate(TimeStep deltaTime) override;
  virtual void OnUIRender(TimeStep deltaTime) override;
  virtual bool OnEvent(const Events::Event &event) override;

  Entity GetActiveCamera() { return m_Scene.GetActiveCamera(); }

 private:
  Application *m_App;
  Scene &m_Scene;
  std::unique_ptr<Shader> shader;
  FrameBuffer &m_ObjectPickerFramebuffer;
  Entity m_EditorCamera;
};

}  // namespace Ham