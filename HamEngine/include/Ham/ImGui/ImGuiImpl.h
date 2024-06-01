#pragma once

#include "Ham/Input/KeyCodes.h"

#include <imgui_internal.h>
#include <ImGuizmo.h>

#include <string>

namespace Ham {
class Window;
class ImGuiImpl {
 public:
  ImGuiImpl();
  ~ImGuiImpl();

  void Init(Window *window);
  void NewFrame();
  void Render();
  void UpdateWindows();
  void SetupPreRender();

  bool WantsCaptureMouse() { return ImGui::GetIO().WantCaptureMouse || ImGuizmo::IsUsingAny(); }
  bool WantsCaptureKeyboard() { return ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantTextInput; }

  bool WantsCaptureKey(KeyCode key)
  {
    switch (key) {
      case KeyCode::LEFT_CONTROL:
      case KeyCode::RIGHT_CONTROL:
      case KeyCode::LEFT_ALT:
      case KeyCode::RIGHT_ALT:
      case KeyCode::LEFT_SHIFT:
      case KeyCode::RIGHT_SHIFT:
      case KeyCode::SPACE:
        return ImGui::GetIO().WantTextInput;
    }

    if ((key >= KeyCode::A && key <= KeyCode::Z) || (key >= KeyCode::DIGIT_0 && key <= KeyCode::DIGIT_9) || (key >= KeyCode::KEYPAD_0 && key <= KeyCode::KEYPAD_9) || (key >= KeyCode::F1 && key <= KeyCode::F12)) {
      return ImGui::GetIO().WantTextInput;
    }
    else {
      return ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantTextInput;
    }
  }

 private:
  Window *m_Window;
  ImGuiSettingsHandler m_ImGuiSettingsHandler;

  std::string settings_file_name = "config.ini";
  std::string settings_file_path;
};
}  // namespace Ham
