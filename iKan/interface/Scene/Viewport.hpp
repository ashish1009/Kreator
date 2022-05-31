//
//  Viewport.hpp
//  iKan
//
//  Created by iKan on 31/05/22.
//

#pragma once

namespace iKan {
    
    class FrameBuffer;
    class Entity;
    
    /// Default Viewport Data
    struct Viewport {
    public:
        /// Flag to check is Viewport Hovered or Focused
        bool Focused = false, Hovered = false;
        
        /// Viewport Height and Width
        float Width = 0.0f, Height = 0.0f;
        
        /// Mouse position inside the Viewoprt
        int32_t MousePosX = -1, MousePosY = -1;
        
        /// Size of Viewport with Title Bar
        glm::vec2 Size;
        
        /// Position of Top Left and Bottom Right edge of Viewport
        glm::vec2 Bounds[2];
        
        /// Cursor Position of Viewport
        ImVec2 CursorPos;
        
        /// Frame buffer to render Inside Viewport
        std::shared_ptr<iKan::FrameBuffer> FrameBuffer;
        
        /// Entity and ID hovered by mouse position
        int32_t HoveredEntityID = -1;
        
        /// Entity Handler to store Viewport
        std::shared_ptr<iKan::Entity> HoveredEntity = nullptr;
        
        int32_t GizmoType = -1;

        // Member functions
        /// Update the Mouse position inside the viewport
        /// To call each frame
        void UpdateMousePos();
        
        /// Update the porition of Viewport bound
        /// To call each frame
        void UpdateBound();
        
        /// render Imgui
        void RenderImgui();
    };
    
}
