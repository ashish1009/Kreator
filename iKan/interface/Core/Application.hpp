//
//  Application.hpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

namespace iKan {
    
    /// Application Core Base class. Responsible for
    /// Initializing all renderer, layers and Window.
    /// Render Data each frame.
    /// Handle Events interrut
    /// Render GUI window for Interface
    class Application {
    public:
        Application();
        virtual ~Application();
    };
    
}
