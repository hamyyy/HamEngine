#pragma once

#include "Ham/Events/EventBase.h"
#include "Ham/Input/Input.h"

namespace Ham::Events {

class MouseMoved : public Event {
 public:
  MouseMoved(float x, float y) : m_X(x), m_Y(y) {}

  float GetX() const { return m_X; }
  float GetY() const { return m_Y; }

  const char* GetName() const override { return "MouseMoved"; }

 private:
  float m_X, m_Y;
};

class MouseScrolled : public Event {
 public:
  MouseScrolled(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

  float GetXOffset() const { return m_XOffset; }
  float GetYOffset() const { return m_YOffset; }

  const char* GetName() const override { return "MouseScrolled"; }

 private:
  float m_XOffset, m_YOffset;
};

class MousePressed : public Event {
 public:
  MousePressed(Ham::MouseButton button) : m_Button(button) {}

  Ham::MouseButton GetButton() const { return m_Button; }

  const char* GetName() const override { return "MousePressed"; }

 private:
  Ham::MouseButton m_Button;
};

class MouseReleased : public Event {
 public:
  MouseReleased(Ham::MouseButton button) : m_Button(button) {}

  Ham::MouseButton GetButton() const { return m_Button; }

  const char* GetName() const override { return "MouseReleased"; }

 private:
  Ham::MouseButton m_Button;
};

class MouseClicked : public Event {
 public:
  struct ClickData {
    Ham::MouseButton button;
    int tollerance = 2;              // amount of pixels the mouse can move while pressed before click is invalid
    uint32_t doubleClickTime = 250;  // ms
    bool isPressed = false;
    bool isDragging = false;
    math::vec2 pos = {0, 0};
    int clickCount = 0;
    uint32_t timer = 0;
    uint32_t timeSinceLastClick = 0;
  };
  MouseClicked(const ClickData& clickData) : m_Button(clickData.button), m_Pos(clickData.pos), m_ClickCount(clickData.clickCount), m_TimeSinceLastClick(clickData.timeSinceLastClick) {}

  Ham::MouseButton GetButton() const { return m_Button; }
  math::vec2 GetPos() const { return m_Pos; }
  int GetClickCount() const { return m_ClickCount; }
  uint32_t GetTimeSinceLastClick() const { return m_TimeSinceLastClick; }

  const char* GetName() const override { return "MouseClicked"; }

 private:
  const Ham::MouseButton& m_Button;
  const math::vec2& m_Pos;
  const int& m_ClickCount;
  const uint32_t& m_TimeSinceLastClick;
};

class MouseDragged : public Event {
 public:
  struct DragData {
    Ham::MouseButton button;
    int tollerance = 3;  // amount of pixels the mouse can move before drag starts
    bool isPressed = false;
    bool isDragging = false;
    math::vec2 startPos = {0, 0};
    math::vec2 pos = {0, 0};
    math::vec2 delta = {0, 0};
    math::vec2 deltaThisFrame = {0, 0};
  };

 public:
  MouseDragged(const DragData& dragData)
      : m_Button(dragData.button),
        m_StartPos(dragData.startPos),
        m_Pos(dragData.pos),
        m_Delta(dragData.delta),
        m_DeltaThisFrame(dragData.deltaThisFrame) {}

  Ham::MouseButton GetButton() const { return m_Button; }
  const math::vec2& GetStartPos() const { return m_StartPos; }
  const math::vec2& GetPos() const { return m_Pos; }
  const math::vec2& GetDelta() const { return m_Delta; }
  const math::vec2& GetDeltaThisFrame() const { return m_DeltaThisFrame; }

  const char* GetName() const override { return "MouseDragged"; }

 private:
  const Ham::MouseButton& m_Button;
  const math::vec2& m_StartPos;
  const math::vec2& m_Pos;
  const math::vec2& m_Delta;
  const math::vec2& m_DeltaThisFrame;
};

class KeyPressed : public Event {
 public:
  KeyPressed(Ham::KeyCode key, int repeatCount) : m_Key(key), m_RepeatCount(repeatCount) {}

  Ham::KeyCode GetKey() const { return m_Key; }
  int GetRepeatCount() const { return m_RepeatCount; }

  const char* GetName() const override { return "KeyPressed"; }

 private:
  Ham::KeyCode m_Key;
  int m_RepeatCount;
};

class KeyReleased : public Event {
 public:
  KeyReleased(Ham::KeyCode key) : m_Key(key) {}

  Ham::KeyCode GetKey() const { return m_Key; }

  const char* GetName() const override { return "KeyReleased"; }

 private:
  Ham::KeyCode m_Key;
};

class WindowResized : public Event {
 public:
  WindowResized(int width, int height) : m_Width(width), m_Height(height) {}

  int GetWidth() const { return m_Width; }
  int GetHeight() const { return m_Height; }

  const char* GetName() const override { return "WindowResize"; }

 private:
  int m_Width, m_Height;
};

class WindowClosed : public Event {
 public:
  const char* GetName() const override { return "WindowClosed"; }
};

class WindowFocused : public Event {
 public:
  const char* GetName() const override { return "WindowFocused"; }
};

class WindowBlurred : public Event {
 public:
  const char* GetName() const override { return "WindowBlurred"; }
};

class WindowMoved : public Event {
 public:
  WindowMoved(int x, int y) : m_X(x), m_Y(y) {}

  int GetX() const { return m_X; }
  int GetY() const { return m_Y; }

  const char* GetName() const override { return "WindowMoved"; }

 private:
  int m_X, m_Y;
};

class KeyTyped : public Event {
 public:
  KeyTyped(uint32_t key) : m_Key(key) {}

  uint32_t GetASCII() const { return m_Key; }

  const char* GetName() const override { return "KeyTyped"; }

 private:
  uint32_t m_Key;
};

class WindowMaximized : public Event {
 public:
  const char* GetName() const override { return "WindowMaximized"; }
};

class WindowMinimized : public Event {
 public:
  const char* GetName() const override { return "WindowMinimized"; }
};

class WindowRestored : public Event {
 public:
  const char* GetName() const override { return "WindowRestored"; }
};

class FileDropped : public Event {
 public:
  FileDropped(int count, const char** paths) : m_Count(count), m_Paths(paths) {}

  int GetCount() const { return m_Count; }
  const char** GetPaths() const { return m_Paths; }

  const char* GetName() const override { return "FileDropped"; }

 private:
  int m_Count;
  const char** m_Paths;
};

class WindowContentScaled : public Event {
 public:
  WindowContentScaled(float xScale, float yScale) : m_XScale(xScale), m_YScale(yScale) {}

  float GetXScale() const { return m_XScale; }
  float GetYScale() const { return m_YScale; }

  const char* GetName() const override { return "WindowContentScaled"; }

 private:
  float m_XScale, m_YScale;
};

class WindowRefreshed : public Event {
 public:
  const char* GetName() const override { return "WindowRefreshed"; }
};

class FramebufferResized : public Event {
 public:
  FramebufferResized(int width, int height) : m_Width(width), m_Height(height) {}

  int GetWidth() const { return m_Width; }
  int GetHeight() const { return m_Height; }

  const char* GetName() const override { return "FramebufferResized"; }

 private:
  int m_Width, m_Height;
};

class JoystickConnected : public Event {
 public:
  JoystickConnected(int jid) : m_JoystickID(jid) {}

  int GetJoystickID() const { return m_JoystickID; }

  const char* GetName() const override { return "JoystickConnected"; }

 private:
  int m_JoystickID;
};

class JoystickDisconnected : public Event {
 public:
  JoystickDisconnected(int jid) : m_JoystickID(jid) {}

  int GetJoystickID() const { return m_JoystickID; }

  const char* GetName() const override { return "JoystickDisconnected"; }

 private:
  int m_JoystickID;
};

class MonitorConnected : public Event {
 public:
  MonitorConnected(void* monitor) : m_Monitor(monitor) {}

  void* GetMonitor() const { return m_Monitor; }

  const char* GetName() const override { return "MonitorConnected"; }

 private:
  void* m_Monitor;
};

class MonitorDisconnected : public Event {
 public:
  MonitorDisconnected(void* monitor) : m_Monitor(monitor) {}

  void* GetMonitor() const { return m_Monitor; }

  const char* GetName() const override { return "MonitorDisconnected"; }

 private:
  void* m_Monitor;
};

}  // namespace Ham::Events