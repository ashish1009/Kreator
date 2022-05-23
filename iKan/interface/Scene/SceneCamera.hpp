//
//  SceneCamera.hpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#pragma once

#include "Renderer/Utils/Camera.hpp"

namespace iKan {
    
    class SceneCamera : public Camera {
    public:
        /// Projection type of Scene Camera
        /// Perspective for 3D
        /// Orthographics for 2D
        enum class ProjectionType { Perspective = 0, Orthographic = 1};
        
        // ------------------------------ Constructors and Destructors --------------------------
        /// Scene Camera constructor
        /// @param projType Projection type
        SceneCamera(ProjectionType projType = ProjectionType::Orthographic);
        
        /// Copy Constructor
        /// @param other other
        SceneCamera(const SceneCamera& other);
        /// Desctructorr
        virtual ~SceneCamera();
        
        // ----------------------------------- Setters ------------------------------------------
        /// Set the Near Plane Overriden Method to update the Projection view matrix
        /// @param near new Near plane
        void SetNear(float near) override;
        /// Set the Far Plane Overriden Method to update the Projection view matrix
        /// @param far new Far plane
        void SetFar(float far) override;
        
        /// Update the new Orthographic Matrix (Only for Orthographic camera)
        /// @param size new size of Camera
        /// @param nearClip new near clip
        /// @param farClip new far clip
        void SetOrthographic(float size, float nearClip, float farClip);
        /// Update the new Perspective Matrix (Only for Perspective camera)
        /// @param fov new FOV
        /// @param nearClip new near clip
        /// @param farClip new far clip
        void SetPerspective(float fov, float nearClip, float farClip);
        
        /// Update the Viewport size of camera
        /// @param width new width
        /// @param height new height
        void SetViewportSize(uint32_t width, uint32_t height);
        
        /// Update the Camera projection type
        /// @param type new projection type
        void SetProjectionType(ProjectionType type);
        /// Set the Size of Orthographic Camera
        /// @param size new size
        void SetOrthographicSize(float size);
        /// Set FOV of Perspective Camera
        /// @param fov new FOV
        void SetPerspectiveFOV(float fov);
        
        // ----------------------------------- Getters ------------------------------------------
        /// return the Projection type of camera
        ProjectionType GetProjectionType() const;
        /// return size of Orthographic Camera
        float GetOrthographicSize() const;
        /// return FOV of Perspective Camera
        float GetPerspectiveFOV() const;
        
        /// Static API to Create instance of Scene Camera
        /// @param projType projection type of camera
        static std::shared_ptr<SceneCamera> Create(ProjectionType projType = ProjectionType::Perspective);
        
    private:
        // Member functions
        void RecalculateProjection();
        
        // Member variables
        ProjectionType m_ProjectionType = ProjectionType::Perspective;
        float m_PerspectiveFOV = glm::radians(45.0f);
        float m_OrthographicSize = 10.0f;
    };
    
}
