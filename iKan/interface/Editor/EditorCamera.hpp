//
//  EditorCamera.hpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#pragma once

#include "Renderer/Utils/Camera.hpp"
#include "Core/Utils/Timestep.hpp"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/ApplicationEvent.h"

namespace iKan {
 
    class EditorCamera : public Camera {
    public:
        // --------------------------- Constructors and Destructor ------------------
        /// Editor Camera Constructor
        EditorCamera();
        /// Editor Camera Destructor
        ~EditorCamera();
        
        /// Update the Parameter of Editor Camera each frame
        /// @param ts time step of each frame
        void Update(Timestep ts);
        /// Handles Events of Editor Camera
        /// @param e event instance
        void EventHandler(Event& e);
        /// Render Gui Data for Editor Camera
        void RendererGui();
        
        /// Mouse Scroll event handler
        /// @param e Mouse Scroll Event
        bool OnMouseScroll(MouseScrolledEvent& e);
        /// Window Resize Event handler
        /// @param e window resize event
        bool OnWindowResize(WindowResizeEvent& e);
        
        /// Update the viewport size for Editor Camera
        /// @param width New width
        /// @param height new height
        void SetViewportSize(uint32_t width, uint32_t height);
       
        // ---------------------- Getters -----------------------------
        /// Return View Projection Matrix
        /// View * Projection
        glm::mat4 GetViewProjection() const;
        /// Return the View Matrix
        const glm::mat4& GetViewMatrix() const;
        
        /// Return the Position of Editor camera
        const glm::vec3& GetPosition() const;
        /// Return the Focal point of Editor camera
        const glm::vec3& GetFocalPoint() const;
        
        /// Return Up Direction of Camera
        glm::vec3 GetUpDirection() const;
        /// Return Right Direction of Camera
        glm::vec3 GetRightDirection() const;
        /// Return Forward Direction of Camera
        glm::vec3 GetForwardDirection() const;
        
        /// Return Zoom level
        float GetZoom() const;

        // ---------------------- Getters -----------------------------
        /// Update the Focal point of camera
        /// @param newFp new Focal point
        void SetFocalPoint(glm::vec3& newFp);
        
        /// Create instance of Editor camera
        static std::shared_ptr<EditorCamera> Create();
        
    private:
        // member functions
        void UpdateCameraView();
        void MouseZoom(float delta);
        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        
        std::pair<float, float> PanSpeed() const;
        
        float RotationSpeed() const;
        float ZoomSpeed() const;
        
        glm::vec3 CalculatePosition() const;
        glm::quat GetOrientation() const;
        
        // Member variables
        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
        
        glm::vec3 m_Position = glm::vec3(0.0f);
        glm::vec3 m_Rotation = glm::vec3(0.0f);
        glm::vec3 m_FocalPoint = glm::vec3(0.0f);
        
        glm::vec2 m_InitialMousePosition = glm::vec2(0.0f);
        
        uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;
        
        float m_FOV = glm::radians(75.0f);
        float m_Distance = 0.0f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;
    };
    
}
