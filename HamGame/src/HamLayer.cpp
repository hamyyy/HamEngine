#include "HamLayer.h"

#include "Ham/Script/CameraController.h"
#include "Ham/Util/ImGuiExtra.h"
#include "Ham/Parser/STLParser.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Ham
{

    std::vector<Component::VertexData> getCubeVertices()
    {
        std::vector<Component::VertexData> vertices = {
            // Front Face
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
            {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
            {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},

            // Right Face
            {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},

            // Back Face
            {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},

            // Left Face
            {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},

            // Top Face
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},

            // Bottom Face
            {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}}};

        return vertices;
    }

    std::vector<unsigned int> getCubeIndices()
    {
        std::vector<unsigned int> indices =
            {
                0, 2, 1, 2, 0, 3,       // Front Face
                4, 6, 5, 6, 4, 7,       // Right Face
                8, 10, 9, 10, 8, 11,    // Back Face
                12, 14, 13, 14, 12, 15, // Left Face
                16, 18, 17, 18, 16, 19, // Top Face
                20, 22, 21, 22, 20, 23  // Bottom Face
            };

        return indices;
    }

    std::vector<Component::VertexData> GetSphereVertices(float radius, int h_segments, int v_segments)
    {
        std::vector<Component::VertexData> vertices;

        float sectorStep = 2 * glm::pi<float>() / h_segments;
        float stackStep = glm::pi<float>() / v_segments;

        for (int i = 0; i <= v_segments; ++i)
        {
            float stackAngle = glm::pi<float>() / 2 - i * stackStep;
            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);

            for (int j = 0; j <= h_segments; ++j)
            {
                float sectorAngle = j * sectorStep;

                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);

                glm::vec3 position(x, y, z);
                glm::vec3 normal(glm::normalize(position));

                vertices.push_back({position, normal});
            }
        }

        return vertices;
    }

    std::vector<unsigned int> GetSphereIndices(int h_segments, int v_segments)
    {
        std::vector<unsigned int> indices;

        int numStacks = v_segments + 1;
        int numSectors = h_segments + 1;

        for (int i = 0; i < numStacks; ++i)
        {
            int k1 = i * numSectors;       // current stack
            int k2 = (i + 1) * numSectors; // next stack

            for (int j = 0; j < numSectors; ++j)
            {
                if (i != 0)
                {
                    indices.push_back(k1 + j);
                    indices.push_back(k2 + j);
                    indices.push_back(k1 + j + 1);
                }

                if (i != numStacks - 1)
                {
                    indices.push_back(k1 + j + 1);
                    indices.push_back(k2 + j);
                    indices.push_back(k2 + j + 1);
                }
            }
        }

        return indices;
    }

    void CalculateNormals(std::vector<Component::VertexData> &vertices, const std::vector<unsigned int> &indices)
    {
        // Initialize normals of all vertices to zero vectors
        for (auto &vertex : vertices)
        {
            vertex.Normal = glm::vec3(0.0f);
        }

        // Calculate face normals and accumulate to vertex normals
        for (size_t i = 0; i < indices.size(); i += 3)
        {
            unsigned int index1 = indices[i];
            unsigned int index2 = indices[i + 1];
            unsigned int index3 = indices[i + 2];

            auto &vertex1 = vertices[index1];
            auto &vertex2 = vertices[index2];
            auto &vertex3 = vertices[index3];

            const glm::vec3 &v1 = vertex1.Position;
            const glm::vec3 &v2 = vertex2.Position;
            const glm::vec3 &v3 = vertex3.Position;

            glm::vec3 faceNormal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

            vertex1.Normal += faceNormal;
            vertex2.Normal += faceNormal;
            vertex3.Normal += faceNormal;
        }

        // Normalize the vertex normals
        for (auto &vertex : vertices)
        {
            vertex.Normal = glm::normalize(vertex.Normal);
        }
    }

    HamLayer::HamLayer(Application *app) : Layer("HamLayer"), m_App(app), m_Scene(m_App->GetScene()) {}

    HamLayer::~HamLayer() {}

    void HamLayer::OnAttach()
    {
        {
            auto entity = m_Scene.CreateEntity("Cube");
            entity.GetComponent<Component::Transform>().Position = glm::vec3(-2.0f, 0.0f, 0.0f);
            auto &shader = entity.AddComponent<Component::Shader>(ASSETS_PATH + "shaders/basic.vert", ASSETS_PATH + "shaders/cube.frag");
            auto &mesh = entity.AddComponent<Component::Mesh>(getCubeVertices(), getCubeIndices());

            // mesh.Indicies.Bind();
            // mesh.Verticies.Bind();

            mesh.Verticies.DefineAttribute3f(offsetof(Component::VertexData, Position));
            mesh.Verticies.DefineAttribute3f(offsetof(Component::VertexData, Normal));

            // mesh.VAO.Unbind();
            // mesh.Verticies.Unbind();
            // mesh.Indicies.Unbind();

            m_Scene.SetSelectedEntity(entity);
        }
        {
            auto entity = m_Scene.CreateEntity("Sphere");
            entity.GetComponent<Component::Transform>().Position = glm::vec3(2.0f, 0.0f, 0.0f);
            auto &shader = entity.AddComponent<Component::Shader>(ASSETS_PATH + "shaders/basic.vert", ASSETS_PATH + "shaders/normals.frag");
            auto &mesh = entity.AddComponent<Component::Mesh>(GetSphereVertices(0.5, 32, 32), GetSphereIndices(32, 32));

            // mesh.Indicies.Bind();
            // mesh.Verticies.Bind();

            mesh.Verticies.DefineAttribute3f(offsetof(Component::VertexData, Position));
            mesh.Verticies.DefineAttribute3f(offsetof(Component::VertexData, Normal));

            mesh.ShowWireframe = true;

            // mesh.VAO.Unbind();
            // mesh.Verticies.Unbind();
            // mesh.Indicies.Unbind();
        }
        {
            auto entity = m_Scene.CreateEntity("Monkey");
            auto &shader = entity.AddComponent<Component::Shader>(ASSETS_PATH + "shaders/basic.vert", ASSETS_PATH + "shaders/funk.frag");

            std::vector<Ham::Component::VertexData> vertices;
            std::vector<unsigned int> indices;
            CalculateNormals(vertices, indices);
            ReadOBJFile(ASSETS_PATH + "models/monkey.obj", vertices, indices);

            auto &mesh = entity.AddComponent<Component::Mesh>(vertices, indices);

            // mesh.Indicies.Bind();
            // mesh.Verticies.Bind();

            mesh.Verticies.DefineAttribute3f(offsetof(Component::VertexData, Position));
            mesh.Verticies.DefineAttribute3f(offsetof(Component::VertexData, Normal));

            // mesh.VAO.Unbind();
            // mesh.Verticies.Unbind();
            // mesh.Indicies.Unbind();
        }
    }

    void HamLayer::OnDetach() {}

    void HamLayer::OnUpdate(TimeStep deltaTime)
    {
        static float speed = 10.0f;
        // if (Input::IsKeyDown(KeyCode::W))
        // {
        //     transform = glm::translate(transform, glm::up() * speed * deltaTime);
        // }

        // if (Input::IsKeyDown(KeyCode::S))
        // {
        //     transform = glm::translate(transform, glm::down() * speed * deltaTime);
        // }

        // if (Input::IsKeyDown(KeyCode::A))
        // {
        //     transform = glm::translate(transform, glm::right() * speed * deltaTime);
        // }

        // if (Input::IsKeyDown(KeyCode::D))
        // {
        //     transform = glm::translate(transform, glm::left() * speed * deltaTime);
        // }

        if (Input::IsKeyDownThisFrame(KeyCode::F))
        {
            auto cameraEntity = GetActiveCamera();
            auto &scriptList = cameraEntity.GetComponent<Component::NativeScriptList>();
            auto script = scriptList.GetScript<CameraController>();

            auto transform = m_Scene.GetSelectedEntity().GetComponent<Component::Transform>().ToMatrix();

            if (cameraEntity != m_Scene.GetSelectedEntity())
                script->SetTarget(transform[3]);
        }

        if (Input::IsKeyDownThisFrame(KeyCode::TAB))
        {
            auto cameraEntity = GetActiveCamera();
            auto &scriptList = cameraEntity.GetComponent<Component::NativeScriptList>();
            auto script = scriptList.GetScript<CameraController>();
            script->SetTarget((glm::vec3(rand() % 1000, rand() % 1000, rand() % 1000) / 1000.0f - 0.5f) * 2.0f);
        }

        // if (Input::IsMouseButtonDownThisFrame(MouseButton::LEFT))
        // {
        //     Input::SetCursorMode(CursorMode::CAPTURED);
        // }

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

        Systems::RenderScene(*m_App, m_Scene, deltaTime);
    }

    void HamLayer::OnUIRender(TimeStep deltaTime)
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

        ImGui::Begin("Ham Engine");
        if (ImGui::Button("Windowed"))
            m_App->SetWindowed();
        if (ImGui::Button("Fullscreen"))
            m_App->SetFullscreen();
        if (ImGui::Button("Fullscreen Borderless"))
            m_App->SetFullscreenBorderless();
        static auto vsync = m_App->IsVSync();
        if (ImGui::Checkbox("VSync", &vsync))
            m_App->SetVSync(vsync);

        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
        static bool enableGizmo = true;
        static bool mouseInUse = false;
        bool useSnap = false;
        float snapValue = 0.25f;
        if (Input::IsKeyDown(KeyCode::LEFT_CONTROL))
            useSnap = true;

        if (Input::IsKeyDown(KeyCode::Q))
            enableGizmo = false;

        ImGui::Checkbox("Gizmos", &enableGizmo);

        if (ImGui::Button("Translate") || Input::IsKeyDown(KeyCode::W))
        {
            enableGizmo = true;
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Rotate") || Input::IsKeyDown(KeyCode::E))
        {
            enableGizmo = true;
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Scale") || Input::IsKeyDown(KeyCode::R))
        {
            enableGizmo = true;
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        }
        if (ImGui::Button("World"))
        {
            enableGizmo = true;
            mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        ImGui::SameLine();
        if (ImGui::Button("Local"))
        {
            enableGizmo = true;
            mCurrentGizmoMode = ImGuizmo::LOCAL;
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

                if (enableGizmo && !mouseInUse)
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
                    ImGui::Checkbox("Show Wireframe", &meshComponent.ShowWireframe);
                }

                if (entity.HasComponent<Component::Camera>())
                {
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

            static std::string text = "Hello World";

            ImGui::LabelText("##TextLabel", "%s", text.c_str());

            static char textValue[256];
            text.resize(IM_ARRAYSIZE(textValue));
            std::strcpy(textValue, text.c_str());
            if (ImGui::InputText("Name", textValue, IM_ARRAYSIZE(textValue)))
            {
                text = textValue;
            }
        }

        ImGui::End();

        ImGui::Begin("Scene Tree");
        ImGui::SceneTree(m_Scene);
        ImGui::End();
    }

    // void HamLayer::OnEvent(Event &event) {}

} // namespace Ham