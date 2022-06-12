//
//  SceneLight.hpp
//  iKan
//
//  Created by iKan on 12/06/22.
//

#pragma once

namespace iKan {
    
    /// Property of Scene Light
    /// Parameter fo this structure should be aligned with shader
    struct SceneLight {
        // NOTE: As Shader glsl takes bool as 4 bytes so we need to use int32 instead of bool
        int32_t Present = 0;
        
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Radiance = { 1.0f, 1.0f, 1.0f };
        
        // Member Methods
        /// Create the instance of Scene Light
        static std::shared_ptr<SceneLight> Create();
        
        /// Render the imgui for Scene Light
        void RenderImgui();
        
        /// Retrurn the transform of Light
        glm::mat4 GetTransform() const;
    };
    
}
