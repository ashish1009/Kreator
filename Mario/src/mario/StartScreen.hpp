//
//  StartScreen.hpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#pragma once

#include "mario/Common.hpp"

namespace Mario {
    
    /// Background Wrapper to store Entity to render background
    class StartScreen {
    public:
        static void Init(const std::shared_ptr<Scene>& scene);
        static void Shutdown(const std::shared_ptr<Scene>& scene);
        
    private:
        static void CreateSubtextures();
    };

}
