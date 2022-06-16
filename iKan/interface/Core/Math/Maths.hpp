//
//  Maths.hpp
//  iKan
//
//  Created by iKan on 05/06/22.
//

#pragma once

#include <glm/glm.hpp>

namespace iKan::Math {
    
    /// Decompose the postion Transform matrix as position rotation and scale
    /// @param transform Transfrom matrix
    /// @param outTranslation Output as position
    /// @param outRotation output as rotation
    /// @param outScale output as scale
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
    
    /// Generate tht transform matrix from position scale and rotation
    /// @param position position
    /// @param rotaiton rotaiton
    /// @param scale scale
    glm::mat4 GetTransformMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    
}
