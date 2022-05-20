//
//  PropertyGrid.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include <imgui_internal.h>

namespace iKan {
    
    class PropertyGrid {
    public:
        /// Render a hint icon and on hovered A hint string will be shown
        /// @param desc String to be used as hint
        /// @param sameLine check for same line icon or in new line
        static void HelpMarker(const char* desc, bool sameLine = true);
    };
    
}
