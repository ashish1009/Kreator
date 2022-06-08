//
//  Component.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "Component.hpp"
#include "Editor/PropertyGrid.hpp"
#include "Renderer/Utils/Renderer.hpp"

using namespace iKan;

IDComponent::IDComponent(const UUID& Id) : ID(Id) {}

TagComponent::TagComponent(const std::string& tag, const std::string& group) : Tag(tag), Group(group) {}

// Transform Component
TransformComponent::TransformComponent(const glm::vec3& translation) : Translation(translation) { }
glm::mat4 TransformComponent::GetTransform() const {
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
    return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}
void TransformComponent::RenderImgui() {
    PropertyGrid::Float3("Translation", Translation, nullptr, 0.25f, 0.0f, 80.0f);
    
    glm::vec3 rotation = glm::degrees(Rotation);
    PropertyGrid::Float3("Rotation", rotation, nullptr, 0.25f, 0.0f, 80.0f);
    Rotation = glm::radians(rotation);
    
    PropertyGrid::Float3("Scale", Scale, nullptr, 0.25f, 1.0f, 80.0f);
}

// Camera Component
CameraComponent::CameraComponent(SceneCamera::ProjectionType projType) : Camera(std::make_shared<SceneCamera>(projType)) { }
void CameraComponent::RenderImgui() {
    auto columnWidth = ImGui::GetWindowContentRegionMax().x / 2;
    ImGui::Columns(2);
    
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Checkbox("Primary", &Primary); ImGui::SameLine();
    
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, columnWidth);
    ImGui::Checkbox("Fixed Aspect Ratio", &FixedAspectRatio);
    
    ImGui::Columns(1);
    ImGui::Separator();
    
    Camera->RenderImgui(true);
}

// Quad Component
QuadComponent::QuadComponent(const TextureComponent& texComp, const glm::vec4& color, float tilingFactor) : Texture(texComp), Color(color), TilingFactor(tilingFactor) {}
QuadComponent::QuadComponent(const std::shared_ptr<iKan::Texture>& texture) { Texture.Component = texture; }
QuadComponent::QuadComponent(const std::string& texturePath) { Texture.Component = Renderer::GetTexture(texturePath);}
QuadComponent::QuadComponent(const glm::vec4& color) : Color(color) {  }
void QuadComponent::RenderImgui(const std::shared_ptr<iKan::Texture>& defaultTexture) {
    // Change the color of the Entity
    ImGui::PushID("Texture");
    ImGui::Columns(2);
    
    ImGui::SetColumnWidth(0, 100);
    size_t texId = (Texture.Component ? Texture.Component->GetRendererID() : defaultTexture->GetRendererID());
    ImGui::Image((void*)texId, ImVec2(40.0f, 40.0f), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
    PropertyGrid::DropConent([this](const std::string& path){
        Texture.Component = Renderer::GetTexture(path);
    });
    
    ImGui::NextColumn();
    ImGui::Checkbox("Albedo Texture", &Texture.Use);
    ImGui::SameLine();
    PropertyGrid::HelpMarker("Drop the Texture file in the Image Button to upload the texture or Select already stored texture from the scene (Option can be available by right click on image)");
    
    ImGui::ColorEdit4("Color", glm::value_ptr(Color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    if (Texture.Component && Texture.Use) {
        ImGui::SameLine();
        ImGui::DragFloat("", &TilingFactor, 1.0f, 1.0f, 1000.0f);
    }
    
    ImGui::Separator();
    
    ImGui::Columns(1);
    ImGui::PopID();

}
