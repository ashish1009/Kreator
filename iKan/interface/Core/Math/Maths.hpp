//
//  Maths.hpp
//  iKan
//
//  Created by iKan on 05/06/22.
//

#pragma once

#include <glm/glm.hpp>

namespace iKan::Math {
    
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
    
}
