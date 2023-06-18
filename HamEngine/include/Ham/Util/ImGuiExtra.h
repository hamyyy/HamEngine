#pragma once

#include "Ham/Scene/Entity.h"
#include "Ham/Scene/Scene.h"
#include "Ham/Scene/Component.h"

#include <glm/glm.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include <string>

namespace ImGui
{

    static bool Transform(const char *label, Ham::Component::Transform &transformComponent)
    {
        bool result = false;
        ImGui::LabelText((std::string("##") + label).c_str(), "%s Transform", label);
        ImGui::PushID(label);
        result |= ImGui::DragFloat3("Position", glm::value_ptr(transformComponent.Position), 0.1f);
        transformComponent.Rotation = glm::degrees(transformComponent.Rotation);
        result |= ImGui::DragFloat3("Rotation", glm::value_ptr(transformComponent.Rotation), 0.1f);
        transformComponent.Rotation = glm::radians(transformComponent.Rotation);
        result |= ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent.Scale), 0.1f);
        ImGui::PopID();

        return result;
    }

    static bool Transform(const char *label, glm::mat4 &transform)
    {
        bool result = false;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 matrix = transform;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matrix), glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale));

        ImGui::LabelText((std::string("##") + label).c_str(), "%s Transform", label);
        ImGui::PushID(label);
        result |= ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
        result |= ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f);
        result |= ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
        ImGui::PopID();

        if (result)
        {
            ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(matrix));
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
        std::sort(entityList.begin(), entityList.end(), [](Ham::Entity &a, Ham::Entity &b) {
            return a.GetComponent<Ham::Component::Tag>().Order < b.GetComponent<Ham::Component::Tag>().Order;
        });
    }

    static void SceneTree(Ham::Scene &scene)
    {
        auto entities = scene.GetTopLevelEntities();
        ImGui::SortEntityList(entities); // TODO: Perform sort only when necessary
        ImGui::EntityNodeRecurse(entities, scene);
    }

} // namespace ImGui
