#include "HamLayer.h"

#include "Ham/Script/CameraController.h"
#include "Ham/Util/ImGuiExtra.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Ham
{

    std::vector<VertexData> getCubeVertices()
    {
        std::vector<VertexData> vertices;

        // Top face
        vertices.push_back({{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}});
        vertices.push_back({{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}});
        vertices.push_back({{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}});
        vertices.push_back({{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}});

        // Back face
        vertices.push_back({{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}});
        vertices.push_back({{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}});
        vertices.push_back({{0.5f, 0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}});
        vertices.push_back({{-0.5f, 0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}});

        // Left face
        vertices.push_back({{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}});
        vertices.push_back({{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}});
        vertices.push_back({{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}});
        vertices.push_back({{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}});

        // Right face
        vertices.push_back({{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}});
        vertices.push_back({{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}});
        vertices.push_back({{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}});
        vertices.push_back({{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}});

        // Top face
        vertices.push_back({{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}});
        vertices.push_back({{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}});
        vertices.push_back({{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}});
        vertices.push_back({{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}});

        // Bottom face
        vertices.push_back({{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}});
        vertices.push_back({{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}});
        vertices.push_back({{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, -1.0f}});
        vertices.push_back({{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, -1.0f}});

        vertices.clear();

        vertices = {
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
            // {
            //     0, 2, 1, 2, 0, 3,       // Top face
            //     4, 5, 6, 6, 7, 4,       // Bottom face
            //     8, 10, 9, 10, 8, 11,    // Right face
            //     12, 13, 14, 14, 15, 12, // Left face
            //     16, 17, 18, 18, 19, 16, // Front face
            //     20, 22, 21, 22, 20, 23  // Back face
            // };

            {
                // Front Face
                0, 1, 2,
                2, 3, 0,

                // Right Face
                4, 5, 6,
                6, 7, 4,

                // Back Face
                8, 9, 10,
                10, 11, 8,

                // Left Face
                12, 13, 14,
                14, 15, 12,

                // Top Face
                16, 17, 18,
                18, 19, 16,

                // Bottom Face
                20, 21, 22,
                22, 23, 20};

        return indices;
    }

    HamLayer::HamLayer(Application *app) : Layer("HamLayer"), m_App(app), m_Scene(m_App->GetScene()) {}

    HamLayer::~HamLayer()
    {
        vb.Destroy();
    }

    void HamLayer::OnAttach()
    {
        // opengl simple cube

        //  create Shader
        shader = std::make_unique<Shader>(ASSETS_PATH + "shaders/cube.vert", ASSETS_PATH + "shaders/cube.frag");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        // struct VertexData
        // {
        //     glm::vec3 position;
        //     glm::vec3 normal;
        // };

        auto vertices = getCubeVertices();
        auto indices = getCubeIndices();

        // Generate and bind the Vertex Array Object (VAO)
        glGenVertexArrays(1, &cubeVAO);
        glBindVertexArray(cubeVAO);

        // Generate and bind the Vertex Buffer Object (VBO) for the vertex data
        vb.Create();
        vb.SetData(vertices);

        // Enable and specify the vertex attribute pointers
        vb.DefineAttribute3f(offsetof(VertexData, position));
        vb.DefineAttribute3f(offsetof(VertexData, normal));

        // Generate and bind the Element Buffer Object (EBO) for the index data
        // unsigned int cubeEBO;
        // glGenBuffers(1, &cubeEBO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        ib.Create();
        ib.SetData(indices);

        // Unbind the VAO and buffers
        glBindVertexArray(0);
        vb.Unbind();
        ib.Unbind();

        transform = glm::mat4(1.0f);
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
            auto cameraEntity = GetCamera();
            auto &scriptList = cameraEntity.GetComponent<Component::NativeScriptList>();
            auto script = scriptList.GetScript<CameraController>();
            script->SetTarget(transform[3]);
        }

        if (Input::IsKeyDownThisFrame(KeyCode::TAB))
        {
            auto cameraEntity = GetCamera();
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

        shader->Bind();

        // transform = glm::rotate(transform, glm::quarter_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
        // Camera
        auto cameraEntity = GetCamera();
        auto &projection = cameraEntity.GetComponent<Component::Camera>().Projection;
        auto view = glm::inverse(cameraEntity.GetComponent<Component::Transform>().ToMatrix());

        shader->SetUniformMat4f("uModel", transform);
        shader->SetUniformMat4f("uView", view);
        shader->SetUniformMat4f("uProjection", projection);

        static auto lightPos = glm::vec3(1.0f, 0.0f, 0.0f);

        // rotate light about z axis
        lightPos = glm::rotate(lightPos, glm::radians(90.0f) * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));

        shader->SetUniform3f("uLightPos", lightPos);
        shader->SetUniform3f("uLightColor", glm::vec3(1.0f, 0.0f, 0.0f));
        shader->SetUniform1f("uTime", m_App->GetTime());
        shader->SetUniform2f("uResolution", m_App->GetWindow().GetSize());

        // render the cube
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        glBindVertexArray(cubeVAO);
        {
            shader->SetUniform3f("uObjectColor", glm::vec3());
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, (int)ib.GetData().size(), GL_UNSIGNED_INT, 0);

            shader->SetUniform3f("uObjectColor", glm::vec3(1, 1, 1));
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, (int)ib.GetData().size(), GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);

        shader->Unbind();
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
        bool useSnap = false;
        float snapValue;
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

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

        auto x = ImGui::GetWindowViewport()->Pos.x;
        auto y = ImGui::GetWindowViewport()->Pos.y;
        auto width = ImGui::GetWindowViewport()->Size.x;
        auto height = ImGui::GetWindowViewport()->Size.y;
        ImGuizmo::SetRect(x, y, width, height);

        if (useSnap)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
            ImGuizmo::SetRect((float)m_App->GetWindow().GetXPos(), (float)m_App->GetWindow().GetYPos(), (float)m_App->GetWindow().GetWidth(), (float)m_App->GetWindow().GetHeight());

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
            {
                auto cameraEntity = GetCamera();
                auto &projection = cameraEntity.GetComponent<Component::Camera>().Projection;
                auto view = glm::inverse(cameraEntity.GetComponent<Component::Transform>().ToMatrix());

                float snapValues[3] = {snapValue, snapValue, snapValue};

                if (enableGizmo)
                    ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(transform), nullptr, useSnap ? snapValues : nullptr);
            }
        }

        {
            auto cameraEntity = GetCamera();
            auto &cameraTransform = cameraEntity.GetComponent<Component::Transform>();

            ImGui::Separator();
            ImGui::Transform("Cube", transform);
            ImGui::Separator();
            ImGui::Transform("Camera", cameraTransform);

            ImGui::Separator();
            ImGui::DragFloat3("Cube Position", glm::value_ptr(transform[3]), 0.1f);

            auto &cameraComponent = cameraEntity.GetComponent<Component::Camera>();
            auto &projection = cameraComponent.Projection;
            auto tf = cameraTransform.ToMatrix();

            auto cameraLookDir = cameraTransform.forward();

            ImGui::Separator();
            ImGui::DragFloat3("Camera Position", glm::value_ptr(cameraTransform.Position), 0.1f);
            ImGui::DragFloat3("Camera Rotation", glm::value_ptr(cameraTransform.Rotation), 0.1f);
            ImGui::DragFloat3("Camera Scale", glm::value_ptr(cameraTransform.Scale), 0.1f);
            ImGui::Separator();

            ImGui::DragFloat3("Camera Look Dir", glm::value_ptr(cameraLookDir), 0.1f);
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
    }

    // void HamLayer::OnEvent(Event &event) {}

} // namespace Ham