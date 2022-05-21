//
//  PropertyGrid.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include <imgui_internal.h>

namespace iKan {
    
    /// Store the Wrapper API for showing Data in Imgui window
    class PropertyGrid {
    public:
        /// Render a hint icon (?) and on hovered A hint string will be shown
        /// @param desc String to be used as hint
        /// @param iconChar Icon to be printed as Help Marker (By default (?) will be renderes)
        /// @param sameLine check for same line icon or in new line
        static void HelpMarker(const char* desc, const char* iconChar = "(?)", bool sameLine = true);
        
    private:
        PropertyGrid() = default;
        MAKE_SINGLETON(PropertyGrid);
    };
    
}
