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
    
}
