//
//  Input.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace iKan {
    
    /// Interface for handling the Input. Implementation in Platform specific in Platform folder
    class Input {
    public:
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseButton button);
        
        static bool IsKeyReleased(KeyCode key);
        static bool IsMouseButtonReleased(MouseButton button);
        
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
        
    private:
        Input() = default;
    };
    
}
