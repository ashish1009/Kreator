//
//  Background.hpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Mario {

    /// Theme color mario tile
    enum ThemeColor {
        Brown = 0, Blue = 1, Grey = 2, Green = 3, MAX_COLOR = 4
    };
    
    class Background {
    public:
        static void Init(const std::shared_ptr<Scene>& scene, ThemeColor color);
        static void Shutdown(const std::shared_ptr<Scene>& scene);
        
    private:
        Background() = default;
        MAKE_SINGLETON(Background);
    };
    
}
