#include "Ham/Editor/EditorLayer.h"

#include "Ham/Script/CameraController.h"
#include "Ham/Util/ImGuiExtra.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Ham
{
    EditorLayer::EditorLayer(Application *app) : Layer("EditorLayer"), m_App(app), m_Scene(m_App->GetScene()), m_ObjectPickerFramebuffer(m_App->GetObjectPickerFramebuffer()) {}

    EditorLayer::~EditorLayer() {}

    void EditorLayer::OnAttach() {}

    void EditorLayer::OnDetach() {}

    void EditorLayer::OnUpdate(TimeStep deltaTime)
    {
        if (Input::IsKeyDownThisFrame(KeyCode::F) && m_Scene.GetSelectedEntity())
        {
            auto cameraEntity = GetActiveCamera();
            auto &scriptList = cameraEntity.GetComponent<Component::NativeScriptList>();
            auto script = scriptList.GetScript<CameraController>();

            auto transform = m_Scene.GetSelectedEntity().GetComponent<Component::Transform>().ToMatrix();

            if (cameraEntity != m_Scene.GetSelectedEntity())
            {
                script->SetTarget(transform[3]);
            }
        }

        if (Input::IsKeyDownThisFrame(KeyCode::ESCAPE))
        {
            Input::SetCursorMode(CursorMode::NORMAL);
        }

        glm::vec2 mouse = Input::GetMousePosition();
        static glm::vec2 prevMouse = mouse;
        static float sensitivity = 0.2f;
        glm::vec2 mouseDelta = mouse - prevMouse;
        prevMouse = mouse;
        if (Input::IsMouseButtonDown(MouseButton::LEFT))
        {
            // local space
            // view = glm::rotate(view, glm::radians(mouseDelta.x * sensitivity), glm::vec3(0.0f, 0.0f, 1.0f));
            // view = glm::rotate(view, glm::radians(-mouseDelta.y * sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));

            // world space
            // transform = glm::rotate(transform, glm::radians(-mouseDelta.y * sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));
            // transform = glm::rotate(transform, glm::radians(mouseDelta.x * sensitivity), glm::vec3(0.0f, 0.0f, 1.0f));
        }
    }

    void EditorLayer::OnUIRender(TimeStep deltaTime)
    {
        // Our state
        static bool show_demo_window = true;
        static bool show_another_window = false;
        static glm::vec4 clear_color = {0.45f, 0.55f, 0.60f, 1.00f};

        ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            auto &io = ImGui::GetIO();

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text. %f", deltaTime.GetMilliseconds()); // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);                        // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                                    // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&m_App->GetWindow().GetClearColor()); // Edit 3 floats representing a color

            static glm::vec4 light = {0.95f, 0.88f, 0.77f, 1.0f};  // light
            static glm::vec4 darker = {0.79f, 0.72f, 0.61f, 1.0f}; // darker but still light
            static glm::vec4 red = {0.84f, 0.16f, 0.22f, 1.0f};    // red
            static glm::vec4 green = {0.44f, 0.51f, 0.43f, 1.0f};  // greenish grey
            static glm::vec4 brown = {0.2f, 0.16f, 0.13f, 1.0f};   // brown (dark)

            ImGui::Separator();
            ImGui::PushID("0");
            if (ImGui::ColorButton("Clear Color", {light.r, light.g, light.b, light.a}))
                m_App->GetWindow().SetClearColor(light);
            ImGui::PopID();
            ImGui::PushID("1");
            ImGui::SameLine();
            if (ImGui::ColorButton("Clear Color", {darker.r, darker.g, darker.b, darker.a}))
                m_App->GetWindow().SetClearColor(darker);
            ImGui::PopID();
            ImGui::PushID("2");
            ImGui::SameLine();
            if (ImGui::ColorButton("Clear Color", {red.r, red.g, red.b, red.a}))
                m_App->GetWindow().SetClearColor(red);
            ImGui::PopID();
            ImGui::PushID("3");
            ImGui::SameLine();
            if (ImGui::ColorButton("Clear Color", {green.r, green.g, green.b, green.a}))
                m_App->GetWindow().SetClearColor(green);
            ImGui::PopID();
            ImGui::PushID("4");
            ImGui::SameLine();
            if (ImGui::ColorButton("Clear Color", {brown.r, brown.g, brown.b, brown.a}))
                m_App->GetWindow().SetClearColor(brown);
            ImGui::PopID();
            ImGui::PushID("5");
            ImGui::SameLine();
            if (ImGui::ColorButton("Clear Color", {0.0f, 0.0f, 0.0f, 1.0f}))
                m_App->GetWindow().SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
            ImGui::PopID();
            ImGui::Separator();

            static bool transparent = false;
            if (ImGui::Checkbox("Transparent Background", &transparent))
            {
                auto cc = m_App->GetWindow().GetClearColor();
                cc.a = transparent ? 0.0f : 1.0f;
                m_App->GetWindow().SetClearColor(cc);
            }

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
        static bool mouseInUse = false;
        bool useSnap = false;
        float snapValue = 0.25f;

        ImGui::Begin("Ham Engine");
        if (ImGui::CollapsingHeader("Application Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::Button("Windowed"))
                m_App->SetWindowed();
            if (ImGui::Button("Fullscreen"))
                m_App->SetFullscreen();
            if (ImGui::Button("Fullscreen Borderless"))
                m_App->SetFullscreenBorderless();
            static auto vsync = m_App->IsVSync();
            if (ImGui::Checkbox("VSync", &vsync))
                m_App->SetVSync(vsync);

            if (Input::IsKeyDown(KeyCode::LEFT_CONTROL))
                useSnap = true;

            if (ImGui::Button("Translate") || Input::IsKeyDown(KeyCode::W))
            {
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("Rotate") || Input::IsKeyDown(KeyCode::E))
            {
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("Scale") || Input::IsKeyDown(KeyCode::R))
            {
                mCurrentGizmoOperation = ImGuizmo::SCALE;
            }
            if (ImGui::Button("World"))
            {
                mCurrentGizmoMode = ImGuizmo::WORLD;
            }
            ImGui::SameLine();
            if (ImGui::Button("Local"))
            {
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            }
        }

        if (useSnap)
        {
            switch (mCurrentGizmoOperation)
            {
            case ImGuizmo::OPERATION::ROTATE:
                snapValue = 45.0f * 0.5f;
                break;
            default:
                snapValue = 0.25f;
                break;
            }
        }

        if (Input::IsMouseButtonDownThisFrame(MouseButton::ANY) && (!ImGuizmo::IsOver() || !ImGuizmo::IsUsing()))
            mouseInUse = true;

        if (Input::IsMouseButtonUpThisFrame(MouseButton::ANY))
            mouseInUse = false;

        mouseInUse = false;

        auto selectedEntity = m_Scene.GetSelectedEntity();

        if (selectedEntity)
        {
            auto &transformComponent = selectedEntity.GetComponent<Component::Transform>();
            auto transform = transformComponent.ToMatrix();

            {
                auto cameraEntity = GetActiveCamera();
                auto &projection = cameraEntity.GetComponent<Component::Camera>().Projection;
                auto view = glm::inverse(cameraEntity.GetComponent<Component::Transform>().ToMatrix());

                float snapValues[3] = {snapValue, snapValue, snapValue};

                if (!mouseInUse)
                {
                    if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(transform), nullptr, useSnap ? snapValues : nullptr))
                    {
                        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(transformComponent.Position), glm::value_ptr(transformComponent.Rotation), glm::value_ptr(transformComponent.Scale));
                        transformComponent.Rotation = glm::radians(transformComponent.Rotation);
                    }
                }
            }
        }

        {
            if (ImGui::CollapsingHeader("Object Picker"))
            {
                auto display = m_App->GetWindow().GetFramebufferSize();
                std::vector<unsigned char> value = m_ObjectPickerFramebuffer.ReadPixels((int)Input::GetMousePosition().x, display.y - (int)Input::GetMousePosition().y, 1, 1);
                uint32_t r = value[0];
                uint32_t g = value[1];
                uint32_t b = value[2];
                uint32_t a = value[3];
                uint32_t id = r + (g << 8) + (b << 16) + (a << 24); // must be uint, dont change to int

                auto image = m_ObjectPickerFramebuffer.GetColorAttachmentID(0);
                auto windowWidth = ImGui::GetWindowSize().x;
                auto windowHeight = windowWidth * ((float)display.y / (float)display.x);
                ImGui::Image((void *)(intptr_t)image, {windowWidth, windowHeight}, ImVec2(0, 1), ImVec2(1, 0));
                ImGui::Text("R: %d, G: %d, B: %d, A: %d", r, g, b, a);
                ImGui::Text("ID: %d", id);
            }
        }

        {
            auto cameraEntity = GetActiveCamera();
            auto &cameraTransform = cameraEntity.GetComponent<Component::Transform>();

            auto entity = m_Scene.GetSelectedEntity();

            if (entity)
            {
                auto &transformComponent = entity.GetComponent<Component::Transform>();
                auto &tagComponent = entity.GetComponent<Component::Tag>();
                auto name = tagComponent.Name.c_str();

                ImGui::Separator();
                ImGui::Transform(name, transformComponent);
                if (entity.HasComponent<Component::Mesh>())
                {
                    auto &meshComponent = entity.GetComponent<Component::Mesh>();
                    auto &shaderList = entity.GetComponent<Component::ShaderList>();
                    bool showNormals = shaderList.Has("vertex-normal");
                    ImGui::Separator();
                    ImGui::LabelText("##Mesh", "%s", "Mesh");
                    ImGui::LabelText("##VertCount", "Vertex Count: %i", meshComponent.Vertices.Size());
                    ImGui::LabelText("##IndexCount", "Index Count: %i", meshComponent.Indices.Size());
                    if (ImGui::Checkbox("Show Normals", &showNormals))
                    {
                        if (showNormals)
                            shaderList.Add("vertex-normal");
                        else
                            shaderList.Remove("vertex-normal");
                    }
                    ImGui::Checkbox("Show Wireframe", &meshComponent.ShowWireframe);
                    ImGui::Checkbox("Show Fill", &meshComponent.ShowFill);
                    ImGui::Checkbox("Enable Alpha Blending", &meshComponent.AlphaBlending);
                    ImGui::Checkbox("Enable Backface Culling", &meshComponent.BackfaceCulling);
                }

                if (entity.HasComponent<Component::Camera>())
                {
                    ImGui::Separator();
                    ImGui::LabelText("##Camera", "%s", "Camera");
                    auto &cameraComponent = entity.GetComponent<Component::Camera>();
                    bool update = false;
                    if (ImGui::DragFloat("Aspect Ratio", &cameraComponent.AspectRatio, 0.1f, 0.1f, 10.0f))
                        update = true;

                    cameraComponent.FOV = glm::degrees(cameraComponent.FOV);
                    if (ImGui::DragFloat("FOV", &cameraComponent.FOV, 0.1f, 0.1f, 180.0f))
                        update = true;
                    cameraComponent.FOV = glm::radians(cameraComponent.FOV);

                    if (ImGui::DragFloat("Near", &cameraComponent.Near, 0.1f, 0.1f, cameraComponent.Far))
                        update = true;
                    if (ImGui::DragFloat("Far", &cameraComponent.Far, 0.1f, cameraComponent.Near, 2000.0f))
                        update = true;

                    if (update)
                        cameraComponent.Update();
                }
            }
        }

        {
            ImGui::Separator();

            static std::string text("Hello, world!", 256);
            ImGui::LabelText("##TextLabel", "%s", text.c_str());
            ImGui::InputText("Name", (char *)text.c_str(), text.size());
        }

        ImGui::End();

        ImGui::Begin("Scene Tree");
        ImGui::SceneTree(m_Scene);
        ImGui::End();
    }

    // void EditorLayer::OnEvent(Event &event) {}

} // namespace Ham