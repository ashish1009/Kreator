//
//  IconRender.hpp
//  Mario
//
//  Created by iKan on 16/06/22.
//


#pragma once

#include "mario/Common.hpp"

namespace Mario {
 
    class IconRender {
    public:
        /// Render Runtime Icon without camera view
        /// @param projection camera projection
        static void RunTimeIcon(const glm::mat4& projection);
        /// Render Runtime Icon without camera view
        /// @param projection camera projection
        static void InitTimeIcon(const glm::mat4& projection, float yPos);
        
    };
    
}
