//
//  Component.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "Component.hpp"

using namespace iKan;

IDComponent::IDComponent(const UUID& Id) : ID(Id) {}

TagComponent::TagComponent(const std::string& tag, const std::string& group) : Tag(tag), Group(group) {}

// Transform Component
TransformComponent::TransformComponent(const glm::vec3& translation) : Translation(translation) { }
glm::mat4 TransformComponent::GetTransform() const {
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
    return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}

// Camera Component
CameraComponent::CameraComponent(SceneCamera::ProjectionType projType) : Camera(std::make_shared<SceneCamera>(projType)) { }

// Quad Component
QuadComponent::QuadComponent(const TextureComponent& texComp, const glm::vec4& color, float tilingFactor) : Texture(texComp), Color(color), TilingFactor(tilingFactor) {}
QuadComponent::QuadComponent(const std::shared_ptr<iKan::Texture>& texture) { Texture.Component = texture; }
QuadComponent::QuadComponent(const std::string& texturePath) { Texture.Component = Renderer::GetTexture(texturePath);}
QuadComponent::QuadComponent(const glm::vec4& color) : Color(color) {  }
