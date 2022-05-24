//
//  AABB.hpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#pragma once

#include <glm/glm.hpp>

namespace iKan {
    
    /// Wrapper to store the AABB Box for any entity
    struct AABB {
        glm::vec3 Min, Max;
        
        /// Constructor
        AABB();
        /// Constructor
        /// @param min Min bound for x, y and Z
        /// @param max Max bound for x, y and Z
        AABB(const glm::vec3& min, const glm::vec3& max);
    };

}
