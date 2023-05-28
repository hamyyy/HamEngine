#include "Ham/Input/Input.h"

#include "Ham/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Ham
{

	bool Input::IsKeyDown(KeyCode keycode)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureKeyboard())
			return false;

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetKey(windowHandle, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		auto &app = Application::Get();

		if (app.GetImGui().WantsCaptureMouse())
			return false;

		GLFWwindow *windowHandle = app.GetWindowHandle();
		int state = glfwGetMouseButton(windowHandle, (int)button);
		return state == GLFW_PRESS;
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

}