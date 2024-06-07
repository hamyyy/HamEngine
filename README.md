# Ham Engine

Ham Engine is a comprehensive game engine written in C++ and OpenGL. It provides a wide range of functionalities for game development, including rendering, input handling, scene management, and more.

Inspired by [Hazel Engine](https://www.github.com/TheCherno/Hazel).

## Core Features

- **ImGui Integration**: Ham Engine integrates with ImGui for GUI rendering, and ImGuizmo for 3D transformation gizmos.
- **Rendering**: The engine supports OpenGL shader and buffer management for rendering.
- **Input Handling**: Input handling is provided by the window manager and the custom event system.
- **Scene Management**: Ham Engine supports scene management with a working entity-component system.
- **Utilities**: The engine includes various utility modules such as `Random`, `TimeStep`, `UUID`, and `File` for various common tasks.
- **Logging and Debugging**: Ham Engine includes a logging system, an assertion system, and a profiler for debugging and performance tracking.
- **Scripting**: The engine supports scripting with Lua.

**Note**: The engine is still a work in progress. Many features are not fully implemented and more will be added in the future.

## Installation

This project uses CMake and makes use of submodules for dependency management.

Clone the repository and include the `Ham.h` header file in your project.

```sh
git clone --recursive https://github.com/hamyyy/HamEngine
```
or 
```sh
git clone https://github.com/hamyyy/HamEngine
git submodule update --init --recursive
```

## Example

Here is an example of how to create a simple scene with a camera and a cube in Ham Engine:

```cpp
#include "Ham.h"

class ExampleLayer : public Ham::Layer
{
public:
   ExampleLayer(): Layer("Example")
   {
      auto camera = Ham::Entity::Create("Camera");
      camera.AddComponent<Ham::CameraComponent>();

      auto cube = Ham::Entity::Create("Cube");
      cube.AddComponent<Ham::MeshComponent>("assets/models/cube.obj");
   }

   void OnUpdate(Ham::TimeStep ts) override
   {
      
   }

   void OnUIUpdate() override
   {
      ImGui::Begin("Settings");
      ImGui::Text("Hello, world!");
      ImGui::End();
   }

   void OnEvent(Ham::Event& e) override
   {
      m_CameraController.OnEvent(e);
   }

private:
      Ham::OrthographicCameraController m_CameraController;
   };

class Sandbox : public Ham::Application
{
public:
   Sandbox()
   {
      PushLayer(new ExampleLayer());
   }

   ~Sandbox()
   {
   }
};

Ham::Application* Ham::CreateApplication()
{
   return new Sandbox();
}
```