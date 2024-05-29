#pragma once

#include "Ham/Input/KeyCodes.h"
#include "Ham/Core/Math.h"

#include <map>

namespace Ham
{
	class Input
	{
	public:
		static void Init();
		static void Shutdown();

		static bool IsKeyDown(KeyCode keycode);
		static bool IsKeyDownThisFrame(KeyCode keycode);
		static bool IsKeyUp(KeyCode keycode);
		static bool IsKeyUpThisFrame(KeyCode keycode);

		static bool IsMouseButtonDown(MouseButton button);
		static bool IsMouseButtonDownThisFrame(MouseButton button);
		static bool IsMouseButtonUp(MouseButton button);
		static bool IsMouseButtonUpThisFrame(MouseButton button);
		static int GetMouseWheelDelta();

		static math::vec2 GetMousePosition();
		static math::vec2 GetPreviousMousePosition();
		static math::vec2 GetMouseDelta();
		static void SetCursorMode(CursorMode mode);

		static void Update();
		static void BeginFrame();
		static void EndFrame();

		static std::map<KeyCode, std::pair<bool, bool>> m_KeyStates;			 // <isDown, wasDown>
		static std::map<MouseButton, std::pair<bool, bool>> m_MouseButtonStates; // <isDown, wasDown>
	private:
		static int s_MouseWheelDelta;
		static int s_MouseWheelChanged;

		static math::vec2 s_PreviousMousePosition;

		static CursorMode s_CurrentCursorMode;
		static CursorMode s_DesiredCursorMode;
	};

}
