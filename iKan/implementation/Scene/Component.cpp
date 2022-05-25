//
//  Component.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "Component.hpp"
#include "Scene/SceneCamera.hpp"

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
CameraComponent::CameraComponent() : Camera(std::make_shared<SceneCamera>()) { }
