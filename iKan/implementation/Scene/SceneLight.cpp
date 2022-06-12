//
//  SceneLight.cpp
//  iKan
//
//  Created by iKan on 12/06/22.
//

#include "SceneLight.hpp"

using namespace iKan;

// Variable outside class to make class same as Shader class
static glm::vec3 LightSize = { 0.5f, 0.5f, 0.5f };

/// Create the instance of Scene Light
std::shared_ptr<SceneLight> SceneLight::Create() {
    return std::make_shared<SceneLight>();
}

/// Render Imgui for Scene light
void SceneLight::ImguiRenderer() {
    bool isLight = (bool)Present;

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0,70);
    
    ImGui::Checkbox("Light", &isLight);
    Present = isLight;
    
    if (isLight) {
        ImGui::NextColumn();
        ImGui::SetColumnWidth(1, 35);
        ImGui::ColorEdit3("", &Radiance.x, ImGuiColorEditFlags_NoInputs);
    }
}

/// Return the transform matrix
glm::mat4 SceneLight::GetTransform() const {
    glm::mat4 rotation = glm::toMat4(glm::quat({ 0.0f, 0.0f, 0.0f }));
    return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), LightSize);
}
