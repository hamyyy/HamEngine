#pragma once

#include <GLFW/glfw3.h>

GLFWmonitor *glfwGetCurrentMonitor(GLFWwindow *window, int *monitorIndex = nullptr);
void glfwCenterWindow(GLFWwindow *window);
bool glfwIsWindowOffScreen(GLFWwindow *window);