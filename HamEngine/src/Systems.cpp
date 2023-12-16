#include "Ham/Scene/Systems.h"

#include "Ham/Core/Base.h"

#include "Ham/Scene/Entity.h"
#include "Ham/Scene/Scene.h"
#include "Ham/Core/Base.h"

namespace Ham
{
    void Systems::AttachNativeScripts(Scene &scene)
    {
        auto view = scene.m_Registry.view<Component::NativeScriptList>();
        for (auto &entity : view)
        {
            auto &list = view.get<Component::NativeScriptList>(entity);
            for (auto &script : list.Scripts)
            {
                if (!script->Active)
                {
                    if (script->World == nullptr)
                        script->World = &scene;
                    if (script->GameObject == nullptr || entity != entt::null)
                        script->GameObject = new Entity(entity, &scene);
                    script->Active = true;
                    script->OnCreate();
                }
            }
        }
    }

    void Systems::DetachNativeScripts(Scene &scene)
    {
        auto view = scene.m_Registry.view<Component::NativeScriptList>();
        for (auto &entity : view)
        {
            auto &list = view.get<Component::NativeScriptList>(entity);
            for (auto &script : list.Scripts)
            {
                if (script->Active)
                {
                    if (script->World == nullptr)
                        script->World = &scene;
                    if (script->GameObject == nullptr || entity != entt::null)
                        script->GameObject = new Entity(entity, &scene);
                    script->OnDestroy();
                    script->Active = false;
                }
            }
        }
    }

    void Systems::UpdateNativeScripts(Scene &scene, TimeStep &deltaTime)
    {
        auto view = scene.m_Registry.view<Component::NativeScriptList>();
        for (auto &entity : view)
        {
            auto &list = view.get<Component::NativeScriptList>(entity);
            for (auto &script : list.Scripts)
            {
                if (script->Active)
                {
                    if (script->World == nullptr)
                        script->World = &scene;
                    if (script->GameObject == nullptr || entity != entt::null)
                        script->GameObject = new Entity(entity, &scene);
                    script->OnUpdate(deltaTime);
                }
            }
        }
    }

    void Systems::UpdateNativeScriptsUI(Scene &scene, TimeStep &deltaTime)
    {
        auto view = scene.m_Registry.view<Component::NativeScriptList>();
        for (auto &entity : view)
        {
            auto &list = view.get<Component::NativeScriptList>(entity);
            for (auto &script : list.Scripts)
            {
                if (script->Active)
                {
                    if (script->World == nullptr)
                        script->World = &scene;
                    if (script->GameObject == nullptr || entity != entt::null)
                        script->GameObject = new Entity(entity, &scene);
                    script->OnUIUpdate(deltaTime);
                }
            }
        }
    }

    void Systems::RenderScene(Application &app, Scene &scene, TimeStep &deltaTime)
    {

        static auto lightPos = glm::vec3(1.0f, 0.0f, 0.0f);
        static auto lightColor = glm::vec3(1.0f, 0.1f, 0.1f);

        auto camview = scene.m_Registry.view<Component::Camera>();
        if (camview.size() == 0)
        {
            HAM_ERROR("No camera in scene!");
            return;
        }

        auto cameraEntity = scene.GetActiveCamera();
        auto &cameraProjection = cameraEntity.GetComponent<Component::Camera>().Projection;
        auto &cameraTransform = cameraEntity.GetComponent<Component::Transform>();
        auto cameraView = glm::inverse(cameraTransform.ToMatrix());

        auto view = scene.m_Registry.view<Component::Mesh, Component::Transform, Component::ShaderList>();

        int index = 0;
        for (auto &ent : view)
        {
            Entity entity = {ent, &scene};

            auto &mesh = entity.GetComponent<Component::Mesh>();
            auto &transform = entity.GetComponent<Component::Transform>();
            auto &shaderList = entity.GetComponent<Component::ShaderList>();
            auto &tag = entity.GetComponent<Component::Tag>();

            for (auto &shaderName : shaderList.Names)
            {
                auto shader = ShaderLibrary::Get(shaderName);

                if (shader == nullptr) // TODO: Use default shader instead
                    continue;

                mesh.VAO.Bind();
                shader->Bind();

                {
                    shader->SetUniformMat4f("uModel", transform.ToMatrix());
                    shader->SetUniformMat4f("uView", cameraView);
                    shader->SetUniformMat4f("uProjection", cameraProjection);

                    shader->SetUniform3f("uLightPos", lightPos);
                    shader->SetUniform3f("uLightColor", lightColor);
                    shader->SetUniform1f("uTime", app.GetTime());
                    shader->SetUniform2f("uResolution", app.GetWindow().GetSize());

                    shader->SetUniform3f("uObjectColor", glm::vec3(1, 1, 1));
                    shader->SetUniform3f("uWireframeColor", glm::vec3());

                    shader->SetUniform1i("uID", index);
                }

                {
                    glEnable(GL_DEPTH_TEST);

                    if (mesh.BackfaceCulling)
                    {
                        glEnable(GL_CULL_FACE);
                        glCullFace(GL_BACK);
                    }
                    else
                    {
                        glDisable(GL_CULL_FACE);
                    }

                    if (mesh.AlphaBlending)
                    {
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        glBlendEquation(GL_FUNC_ADD);
                    }
                    else
                    {
                        glDisable(GL_BLEND);
                    }

                    if (mesh.ShowFill || shaderName == "vertex-normal")
                    {
                        shader->SetUniform1i("uIsWireframe", 0);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        glDrawElements(GL_TRIANGLES, (GLushort)mesh.Indices.GetData().size(), GL_UNSIGNED_INT, 0);
                    }

                    if (mesh.ShowWireframe)
                    {
                        shader->SetUniform1i("uIsWireframe", 1);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        glDrawElements(GL_TRIANGLES, (GLushort)mesh.Indices.GetData().size(), GL_UNSIGNED_INT, 0);
                    }
                }
            }

            index++;
        }

        // rotate light about z axis
        lightPos = glm::rotate(lightPos, glm::radians(90.0f) * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void Systems::RenderObjectPickerFrame(Application &app, Scene &scene, TimeStep &deltaTime)
    {
        auto camview = scene.m_Registry.view<Component::Camera>();
        if (camview.size() == 0)
        {
            HAM_ERROR("No camera in scene!");
            return;
        }

        auto cameraEntity = scene.GetActiveCamera();
        auto &cameraProjection = cameraEntity.GetComponent<Component::Camera>().Projection;
        auto &cameraTransform = cameraEntity.GetComponent<Component::Transform>();
        auto cameraView = glm::inverse(cameraTransform.ToMatrix());

        auto view = scene.m_Registry.view<Component::Mesh>();

        int index = 0;
        for (auto &ent : view)
        {
            Entity entity = {ent, &scene};

            auto &mesh = entity.GetComponent<Component::Mesh>();
            auto &transform = entity.GetComponent<Component::Transform>();
            auto &shaderList = entity.GetComponent<Component::ShaderList>();
            auto &tag = entity.GetComponent<Component::Tag>();
            auto shader = ShaderLibrary::Get("object-picker");

            mesh.VAO.Bind();
            shader->Bind();

            {
                shader->SetUniformMat4f("uModel", transform.ToMatrix());
                shader->SetUniformMat4f("uView", cameraView);
                shader->SetUniformMat4f("uProjection", cameraProjection);

                shader->SetUniform1f("uTime", app.GetTime());
                shader->SetUniform2f("uResolution", app.GetWindow().GetSize());

                shader->SetUniform3f("uObjectColor", glm::vec3(1, 1, 1));
                shader->SetUniform3f("uWireframeColor", glm::vec3());
                shader->SetUniform1i("uID", index);
                shader->SetUniform1i("uTotalObjects", (int)view.size());
            }

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, (GLushort)mesh.Indices.GetData().size(), GL_UNSIGNED_INT, 0);

            index++;
        }
    }

    void Systems::HandleObjectPicker(Application &app, Scene &scene, FrameBuffer &frameBuffer, TimeStep &deltaTime)
    {
        frameBuffer.Bind();
        frameBuffer.Clear(AttachmentType::COLOR | AttachmentType::DEPTH | AttachmentType::STENCIL);
        Systems::RenderObjectPickerFrame(app, scene, deltaTime);
        frameBuffer.Unbind();

        auto display = app.GetWindow().GetFramebufferSize();

        std::vector<unsigned char> value = frameBuffer.ReadPixels((int)Input::GetMousePosition().x, display.y - (int)Input::GetMousePosition().y, 1, 1);
        uint32_t r = value[0];
        uint32_t g = value[1];
        uint32_t b = value[2];
        uint32_t a = value[3];

        uint32_t id = r + (g << 8) + (b << 16) + (a << 24); // must be uint, dont change to int

        auto view = scene.m_Registry.view<Component::Mesh>();
        if (id > view.size())
        {
            scene.ClearHoveredEntity();
        }
        else
        {
            scene.SetHoveredEntity({view[id], &scene});
        }

        if (Input::IsMouseButtonUpThisFrame(MouseButton::LEFT))
        {
            if (id > view.size())
            {
                scene.ClearSelectedEntity();
            }
            else
            {
                scene.SetSelectedEntity({view[id], &scene});
            }
        }
    }
} // namespace Ham
