//
//  Camera.cpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#include "Camera.hpp"

using namespace iKan;

/// Setters
void Camera::SetNear(float near) { m_Near = near; }
void Camera::SetFar(float far) { m_Far = far;}

/// Getters
float Camera::GetNear() const { return m_Near; }
float Camera::GetFar() const { return m_Far; }
const glm::mat4& Camera::GetProjectionMatrix() const { return m_Projection; }
