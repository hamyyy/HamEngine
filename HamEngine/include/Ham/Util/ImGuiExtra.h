#pragma once

#include "Ham/Scene/Component.h"

#include <glm/glm.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

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

} // namespace ImGui
