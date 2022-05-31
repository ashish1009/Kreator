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
        
        /// Create Imgae in current Imgui window
        /// @param textureID Texture id of Image to be rendered
        /// @param size size of image need to be rendered inside Imgui window (this size will be visible as texture)
        /// @param uv0 Left
        /// @param uv1 Right
        static void Image(void* textureID, const glm::vec2& size, const glm::vec2& uv0, const glm::vec2& uv1);
        
    private:
        PropertyGrid() = default;
        MAKE_SINGLETON(PropertyGrid);
    };
    
}
