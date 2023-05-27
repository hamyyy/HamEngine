#include "HamLayer.h"

namespace Ham
{

    HamLayer::HamLayer(Application *app) : Layer("HamLayer"), m_App(app) {}
    HamLayer::~HamLayer() {}

    void HamLayer::OnAttach()
    {
        // opengl hellp triangle

        // vertex shader
        const char *vertexShaderSource = "#version 460 core\n"
                                         "layout (location = 0) in vec3 VERTEX_POS;\n"
                                         "uniform mat4 transform;\n"
                                         "void main()\n"
                                         "{\n"
                                         "gl_Position = transform * vec4(VERTEX_POS.x, VERTEX_POS.y, VERTEX_POS.z, 1.0);\n"
                                         "}\0";
        // fragment shader
        const char *fragmentShaderSource = "#version 460 core\n"
                                           "layout (location = 0) out vec4 FRAG_COLOR;\n"
                                           "vec3 dirLight = vec3(1.0f, 1.0f, 1.0f);\n"
                                           "uniform mat4 transform;\n"
                                           "void main()\n"
                                           "{\n"
                                           //    calculate normal
                                           "vec3 normal = vec3(0.0, 0.0, 1.0);\n"
                                           "normal = normalize(mat3(transform) * normal);\n"
                                           //    calculate light intensity
                                           "float lightIntensity = dot(normalize(dirLight), normalize(vec3(0.0, 0.0, 1.0)));\n"
                                           //    calculate light color
                                           "vec3 lightColor = vec3(1.0f, 1.0f, 1.0f) + vec3(1.0, 0.5, 0.0);\n"
                                           //    calculate final color
                                           "vec3 finalColor = lightIntensity * lightColor;\n"
                                           //    set final color
                                           "FRAG_COLOR = vec4(finalColor, 1.0f);\n"
                                           "}\n\0";

        //  create Shader
        shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float L = 1.0f;
        vertices = {
            {0.0f, 0.0f, 0.0f},     // v0
            {-0.866f, -0.5f, 0.0f}, // v1
            {0.866f, -0.5f, 0.0f},  // v2
            {0.0f, 1.0f, 0.0f},     // v3

            {0.0f, 0.0f, 0.0f},     // v0
            {0.0f, -0.866f, -0.5f}, // v1
            {0.0f, 0.866f, -0.5f},  // v2
            {0.0f, 0.0f, 1.0f},     // v3
        };

        // vertices = {
        //     {0.5, 0.5f, 0.0f},   // v0
        //     {0.5, -0.5f, 0.0f},  // v1
        //     {-0.5, -0.5f, 0.0f}, // v2
        //     {-0.5, 0.5f, 0.0f},  // v3
        // };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        int sizeOfBuffer = (int)vertices.size() * sizeof(glm::vec3);
        glBufferData(GL_ARRAY_BUFFER, sizeOfBuffer, &vertices[0], GL_STATIC_DRAW);

        // position attribute
        int sizeOfSingleVertex = sizeof(glm::vec3);
        int numberOfElementsPerVertex = sizeof(glm::vec3) / sizeof(float);
        glVertexAttribPointer(0, numberOfElementsPerVertex, GL_FLOAT, GL_FALSE, sizeOfSingleVertex, nullptr);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        // uncomment this call to draw in wireframe polygons.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void HamLayer::OnDetach() {}

    void HamLayer::OnUpdate(TimeStep deltaTime)
    {
        shader->Bind();

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f);
        // first translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
        // transform = glm::scale(transform, glm::vec3((float)height / (float)width, 1.0f, 1.0f));
        // transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, m_App->GetTime(), glm::vec3(1.0f, 1.0f, 0.0f));

        auto camera = glm::perspective(glm::radians(45.0f), m_App->GetWindow().GetAspectRatio(), 0.1f, 100.0f);
        auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
        transform = camera * view * transform;

        // get matrix's uniform location and set matrix
        shader->SetUniformMat4f("transform", transform);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size());
        glDrawArrays(GL_LINE_LOOP, 0, (int)vertices.size());
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
        ImGui::End();
    }

    // void HamLayer::OnEvent(Event &event) {}

} // namespace Ham