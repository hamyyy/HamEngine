#pragma once

#include "Ham/Core/Math.h"
#include "Ham/Scene/Entity.h"
#include "Ham/Scene/Scene.h"
#include "Ham/Scene/Component.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <string>

namespace ImGui
{
    using namespace Ham;

    static bool Transform(const char *label, Ham::Component::Transform &transformComponent)
    {
        bool result = false;
        ImGui::LabelText((std::string("##") + label).c_str(), "%s Transform", label);
        ImGui::PushID(label);
        result |= ImGui::DragFloat3("Position", transformComponent.Position.data(), 0.1f);
        transformComponent.Rotation = math::degrees(transformComponent.Rotation);
        result |= ImGui::DragFloat3("Rotation", transformComponent.Rotation.data(), 0.1f);
        transformComponent.Rotation = math::radians(transformComponent.Rotation);
        result |= ImGui::DragFloat3("Scale", transformComponent.Scale.data(), 0.1f);
        ImGui::PopID();

        return result;
    }

    static bool Transform(const char *label, math::mat4 &transform)
    {
        bool result = false;

        math::vec3 position;
        math::vec3 rotation;
        math::vec3 scale;
        math::mat4 matrix = transform;
        ImGuizmo::DecomposeMatrixToComponents(matrix.stripes.data()->data(), position.data(), rotation.data(), scale.data());

        ImGui::LabelText((std::string("##") + label).c_str(), "%s Transform", label);
        ImGui::PushID(label);
        result |= ImGui::DragFloat3("Position", position.data(), 0.1f);
        result |= ImGui::DragFloat3("Rotation", rotation.data(), 0.1f);
        result |= ImGui::DragFloat3("Scale", scale.data(), 0.1f);
        ImGui::PopID();

        if (result)
        {
            ImGuizmo::RecomposeMatrixFromComponents(position.data(), rotation.data(), scale.data(), matrix.stripes.data()->data());
            transform = matrix;
        }

        return result;
    }

    static void EntityNodeRecurse(std::vector<Ham::Entity> &entities, Ham::Scene &scene)
    {
        for (auto &entity : entities)
        {
            auto id = std::string("##") + entity.GetUUID();
            auto children = entity.GetChildren();

            uint32_t flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            if (children.empty())
                flags |= ImGuiTreeNodeFlags_Leaf;

            if (entity == scene.GetSelectedEntity())
                flags |= ImGuiTreeNodeFlags_Selected;

            bool node_open = ImGui::TreeNodeEx(id.c_str(), flags, entity.GetName().c_str());

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                scene.SetSelectedEntity(entity);

            if (node_open)
            {
                ImGui::EntityNodeRecurse(children, scene);
                ImGui::TreePop();
            }
        }
    }

    static void SortEntityList(std::vector<Ham::Entity> &entityList)
    {
        std::sort(entityList.begin(), entityList.end(), [](Ham::Entity &a, Ham::Entity &b)
                  { return a.GetComponent<Ham::Component::Tag>().Order < b.GetComponent<Ham::Component::Tag>().Order; });
    }

    static void SceneTree(Ham::Scene &scene)
    {
        auto entities = scene.GetTopLevelEntities();
        ImGui::SortEntityList(entities); // TODO: Perform sort only when necessary
        ImGui::EntityNodeRecurse(entities, scene);
    }

} // namespace ImGui
