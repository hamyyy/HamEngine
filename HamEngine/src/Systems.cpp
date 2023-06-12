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

        auto view = scene.m_Registry.view<Component::Mesh, Component::Transform, Component::Shader>();

        int index = 0;
        for (auto &ent : view)
        {
            Entity entity = {ent, &scene};

            auto &mesh = entity.GetComponent<Component::Mesh>();
            auto &transform = entity.GetComponent<Component::Transform>();
            auto &shader = entity.GetComponent<Component::Shader>();
            auto &tag = entity.GetComponent<Component::Tag>();

            shader.Bind();
            mesh.VAO.Bind();

            {
                shader.SetUniformMat4f("uModel", transform.ToMatrix());
                shader.SetUniformMat4f("uView", cameraView);
                shader.SetUniformMat4f("uProjection", cameraProjection);

                shader.SetUniform3f("uLightPos", lightPos);
                shader.SetUniform3f("uLightColor", lightColor);
                shader.SetUniform1f("uTime", app.GetTime());
                shader.SetUniform2f("uResolution", app.GetWindow().GetSize());

                shader.SetUniform3f("uObjectColor", glm::vec3(1, 1, 1));
                shader.SetUniform3f("uWireframeColor", glm::vec3());

                shader.SetUniform1i("uID", index);
            }

            {
                if (mesh.ShowFill)
                {
                    glEnable(GL_DEPTH_TEST);
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    // alpha blending
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                    shader.SetUniform1i("uIsWireframe", 0);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glDrawElements(GL_TRIANGLES, (GLushort)mesh.Indicies.GetData().size(), GL_UNSIGNED_INT, 0);
                }

                if (mesh.ShowWireframe)
                {
                    glDisable(GL_BLEND);

                    shader.SetUniform1i("uIsWireframe", 1);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glDrawElements(GL_TRIANGLES, (GLushort)mesh.Indicies.GetData().size(), GL_UNSIGNED_INT, 0);
                }
            }

            index++;
        }

        // rotate light about z axis
        lightPos = glm::rotate(lightPos, glm::radians(90.0f) * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
    }
} // namespace Ham
