#include "Ham/Input/Input.h"

#include "Ham/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Ham
{

	std::map<KeyCode, std::pair<bool, bool>> Input::m_KeyStates;
	std::map<MouseButton, std::pair<bool, bool>> Input::m_MouseButtonStates;

	bool Input::IsKeyDown(KeyCode keycode)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureKeyboard())
			return false;

		if (m_KeyStates.find(keycode) == m_KeyStates.end())
			m_KeyStates[keycode] = {false, false};

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetKey(windowHandle, (int)keycode);
		m_KeyStates[keycode].first = state == GLFW_PRESS || state == GLFW_REPEAT;
		return m_KeyStates[keycode].first;
	}

	bool Input::IsKeyDownThisFrame(KeyCode keycode)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureKeyboard())
			return false;

		if (m_KeyStates.find(keycode) == m_KeyStates.end())
			m_KeyStates[keycode] = {false, false};

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetKey(windowHandle, (int)keycode);
		m_KeyStates[keycode].first = state == GLFW_PRESS || state == GLFW_REPEAT;
		return m_KeyStates[keycode].first && !m_KeyStates[keycode].second;
	}

	bool Input::IsKeyUp(KeyCode keycode)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureKeyboard())
			return false;

		if (m_KeyStates.find(keycode) == m_KeyStates.end())
			m_KeyStates[keycode] = {false, false};

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetKey(windowHandle, (int)keycode);
		m_KeyStates[keycode].first = state == GLFW_RELEASE || state == GLFW_REPEAT;
		return !m_KeyStates[keycode].first;
	}

	bool Input::IsKeyUpThisFrame(KeyCode keycode)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureKeyboard())
			return false;

		if (m_KeyStates.find(keycode) == m_KeyStates.end())
			m_KeyStates[keycode] = {false, false};

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetKey(windowHandle, (int)keycode);
		m_KeyStates[keycode].first = state == GLFW_PRESS || state == GLFW_REPEAT;
		return !m_KeyStates[keycode].first && m_KeyStates[keycode].second;
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureMouse())
			return false;

		if (m_MouseButtonStates.find(button) == m_MouseButtonStates.end())
			m_MouseButtonStates[button] = {false, false};

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetMouseButton(windowHandle, (int)button);
		m_MouseButtonStates[button].first = state == GLFW_PRESS;
		return m_MouseButtonStates[button].first;
	}

	bool Input::IsMouseButtonDownThisFrame(MouseButton button)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureMouse())
			return false;

		if (m_MouseButtonStates.find(button) == m_MouseButtonStates.end())
			m_MouseButtonStates[button] = {false, false};

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetMouseButton(windowHandle, (int)button);
		m_MouseButtonStates[button].first = state == GLFW_PRESS;
		return m_MouseButtonStates[button].first && !m_MouseButtonStates[button].second;
	}

	bool Input::IsMouseButtonUp(MouseButton button)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureMouse())
			return false;

		if (m_MouseButtonStates.find(button) == m_MouseButtonStates.end())
			m_MouseButtonStates[button] = {false, false};

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetMouseButton(windowHandle, (int)button);
		m_MouseButtonStates[button].first = state == GLFW_PRESS;
		return !m_MouseButtonStates[button].first;
	}

	bool Input::IsMouseButtonUpThisFrame(MouseButton button)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureMouse())
			return false;

		if (m_MouseButtonStates.find(button) == m_MouseButtonStates.end())
			m_MouseButtonStates[button] = {false, false};

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetMouseButton(windowHandle, (int)button);
		m_MouseButtonStates[button].first = state == GLFW_PRESS;
		return !m_MouseButtonStates[button].first && m_MouseButtonStates[button].second;
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow *windowHandle = Application::Get().GetWindowHandle();

		double x, y;
		glfwGetCursorPos(windowHandle, &x, &y);
		return {(float)x, (float)y};
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		GLFWwindow *windowHandle = Application::Get().GetWindowHandle();
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
	}

	void Input::BeginFrame()
	{
	}

	void Input::EndFrame()
	{
		for (auto &key : m_KeyStates)
		{
			key.second.second = key.second.first;
		}

		for (auto &button : m_MouseButtonStates)
		{
			button.second.second = button.second.first;
		}
	}

}