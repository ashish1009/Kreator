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
    
    class Entity;
    class Mesh;
    
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
    
    // TODO: Make Color and TextureComponent in base class
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
        
        void RenderImgui(const std::shared_ptr<iKan::Texture>& defaultTexture);
    };
    
    /// Stores the Quad Component
    struct CircleComponent {
        TextureComponent Texture;
        glm::vec4 Color = glm::vec4(1.0f);
        float TilingFactor = 1.0f;
        float Thickness = 1.0f;
        float Fade = 0.005f;
        
        CircleComponent(const CircleComponent& other);
        CircleComponent(float thickness = 1.0f, float fade = 0.005);
        CircleComponent(const TextureComponent& texComp, float thickness = 1.0f, float fade = 0.005, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
        CircleComponent(const std::shared_ptr<iKan::Texture>& texture);
        CircleComponent(const std::string& texturePath);
        
        void RenderImgui(const std::shared_ptr<iKan::Texture>& defaultTextur);
    };

    /// Mesh
    struct MeshComponent {
        std::shared_ptr<Mesh> Mesh = nullptr;
        
        MeshComponent() = default;
        MeshComponent(const MeshComponent& other);
        void RenderImgui(const std::shared_ptr<iKan::Texture>& defaultTexture, const std::shared_ptr<Entity>& entity);
    };
    
    /// Sprite component for storing the subtexture
    // TODO: Add ImguiRenderer, Scene Serializer and Scene Hierarcy pannel later
    struct SpriteComponent {
        std::shared_ptr<iKan::SubTexture> SubTexture;
    };

}
