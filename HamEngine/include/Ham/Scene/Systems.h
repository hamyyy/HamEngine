#pragma once
#include "Ham.h"

#include "Ham/Scene/Component.h"
#include "Ham/Renderer/FrameBuffer.h"

namespace Ham {
class Systems {
 public:
  static void AttachNativeScripts(Scene &scene);
  static void AttachNativeScriptsForEntity(Scene &scene, Entity entity);
  static void DetachNativeScripts(Scene &scene);
  static void UpdateNativeScripts(Scene &scene, TimeStep &deltaTime);
  static void UpdateNativeScriptsUI(Scene &scene, TimeStep &deltaTime);
  static void RenderScene(Application &app, Scene &scene, TimeStep &deltaTime);
  static void RenderObjectPickerFrame(Application &app, Scene &scene, TimeStep &deltaTime);
  static void HandleObjectPicker(Application &app, Scene &scene, FrameBuffer &frameBuffer, TimeStep &deltaTime);
};
}  // namespace Ham
