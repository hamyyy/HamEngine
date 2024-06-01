#include "Ham/Util/GLFWExtra.h"
#include "Ham/Core/Assert.h"

#include <cassert>

GLFWmonitor *glfwGetCurrentMonitor(GLFWwindow *window, int *monitorIndex)
{
  int monitorCount;
  auto monitors = glfwGetMonitors(&monitorCount);

  HAM_CORE_ASSERT(monitorCount > 0, "No monitors found");

  auto monitor = monitors[0];
  int x, y, width, height;
  for (int i = 0; i < monitorCount; i++) {
    glfwGetMonitorPos(monitors[i], &x, &y);
    auto videoMode = glfwGetVideoMode(monitors[i]);
    width = videoMode->width;
    height = videoMode->height;

    int posX, posY;
    glfwGetWindowPos(window, &posX, &posY);

    if (posX >= x && posX < x + width && posY >= y && posY < y + height) {
      monitor = monitors[i];
      if (monitorIndex)
        *monitorIndex = i;
      // std::cout << "Found monitor (" << i << ") position: (" << x << "," << y << ") size: (" << width << "," << height << ")" << std::endl;
      break;
    }
  }

  return monitor;
}

void glfwCenterWindow(GLFWwindow *window)
{
  int x, y, width, height;
  int windowWidth, windowHeight;
  auto isMaximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
  glfwGetMonitorWorkarea(glfwGetCurrentMonitor(window), &x, &y, &width, &height);
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glfwSetWindowPos(window, x + width / 2 - windowWidth / 2, y + height / 2 - windowHeight / 2);
}

bool glfwIsWindowOffScreen(GLFWwindow *window)
{
  int x, y, width, height;
  int windowWidth, windowHeight;
  glfwGetMonitorWorkarea(glfwGetCurrentMonitor(window), &x, &y, &width, &height);
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  int posX, posY;
  glfwGetWindowPos(window, &posX, &posY);
  return posX < x || posX + windowWidth > x + width || posY < y || posY + windowHeight > y + height;
}