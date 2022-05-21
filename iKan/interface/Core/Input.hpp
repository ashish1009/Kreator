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
    /// Resposible for checking the Type of input given to Window or application (Either Key or Mouse)
    class Input {
    public:
        /// Check is a specific key is pressed, return true if pressed
        /// @param key keycode to be checked
        static bool IsKeyPressed(KeyCode key);
        /// Check is a specific Mouse button is pressed, return true if pressed
        /// @param button mouse button code to be checked
        static bool IsMouseButtonPressed(MouseButton button);
        
        /// Check is a specific key is released, return true if released
        /// @param key keycode to be checked
        static bool IsKeyReleased(KeyCode key);
        /// Check is a specific key is released, return true if released
        /// @param button mouse button code to be checked
        static bool IsMouseButtonReleased(MouseButton button);
        
        /// Return the Mouse position (X, Y) in pair
        static std::pair<MOUSE_POS_X_TYPE, MOUSE_POS_Y_TYPE> GetMousePosition();
        static MOUSE_POS_X_TYPE GetMouseX();
        static MOUSE_POS_Y_TYPE GetMouseY();
        
    private:
        Input() = default;
        MAKE_SINGLETON(Input);
    };
    
}
