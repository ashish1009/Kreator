//
//  Background.hpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#pragma once

#include "mario/Common.hpp"

namespace Mario {

    /// Theme color mario tile
    enum ThemeColor {
        Brown = 0, Blue = 1, Grey = 2, Green = 3, MAX_COLOR = 4
    };
    
    class Background {
    public:
        /// Initialize the Background tiles and Create their entity
        /// @param scene Scene reference pointer
        /// @param color Theme color
        static void Init(const std::shared_ptr<Scene>& scene, ThemeColor color);
        /// Shut down the Background. Delete all entity if created
        /// @param scene Scene referece pointer
        static void Shutdown(const std::shared_ptr<Scene>& scene);
        
        /// Render Runtime Icon without camera view
        /// @param projection camera projection
        static void RuntimeIcon(const glm::mat4& projection);
        
    private:
        Background() = default;
        MAKE_SINGLETON(Background);
    };
    
}
