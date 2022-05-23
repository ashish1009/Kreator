//
//  Camera.h
//  iKan
//
//  Created by iKan on 23/05/22.
//

#pragma once

namespace iKan {
    
    /// Base Camera class. both Scene and Editor camera derived from this base class
    class Camera {
    public:
        Camera() = default;
        virtual ~Camera() = default;
        
        /// Update the Near plane of camera
        /// @param near New near plane
        virtual void SetNear(float near);
        /// Update the Far plane of camera
        /// @param far new fat plane
        virtual void SetFar(float far);
        
        /// Return the Near Plane value
        float GetNear() const;
        /// Return the Far Plane value
        float GetFar() const;
        /// Return the Projection Matrix of Camera
        const glm::mat4& GetProjectionMatrix() const;
        
    protected:
        float m_Near = 0.0f;
        float m_Far = 0.0f;
        float m_AspectRatio = 16.0f / 9.0f;
        
        glm::mat4 m_Projection = glm::mat4(1.0f);
    };

}
