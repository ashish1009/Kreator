//
//  Component.hpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#pragma once

#include "Core/Math/UUID.hpp"
#include "Scene/SceneCamera.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace iKan {
    
    /// Store the unique ID of Entity
    struct IDComponent {
        UUID ID = 0;
        IDComponent(const UUID& Id);
    };

    /// store the title of Entity
    struct TagComponent {
        std::string Tag = "Default Entity";
        std::string Group = "None";
        TagComponent(const std::string& tag, const std::string& group = "None");
    };
    
    /// Sotre the Local Position of Entity
    struct TransformComponent {
        glm::vec3 Translation   = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation      = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale         = { 1.0f, 1.0f, 1.0f };
        
        TransformComponent(const glm::vec3& translation = { 0.0f, 0.0f, 0.0f });
        glm::mat4 GetTransform() const;
        
        void RenderImgui();
    };

    /// Scene Camera
    struct CameraComponent {
        bool FixedAspectRatio = false;
        bool Primary = true;
        std::shared_ptr<SceneCamera> Camera;
        
        CameraComponent(SceneCamera::ProjectionType projType);
        
        void RenderImgui();
    };
    
    /// Stores the Quad Component
    struct QuadComponent {
        TextureComponent Texture;
        glm::vec4 Color = glm::vec4(1.0f);
        float TilingFactor = 1.0f;
        
        QuadComponent() = default;
        QuadComponent(const TextureComponent& texComp, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
        QuadComponent(const std::shared_ptr<iKan::Texture>& texture);
        QuadComponent(const std::string& texturePath);
        QuadComponent(const glm::vec4& color);
        
        void RenderImgui();
    };
    
}
