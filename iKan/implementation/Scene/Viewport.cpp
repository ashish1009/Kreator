//
//  Viewport.cpp
//  iKan
//
//  Created by iKan on 31/05/22.
//

#include "Viewport.hpp"

using namespace iKan;

/// update Mouse position
void Viewport::UpdateMousePos() {
    auto [mx, my] = ImGui::GetMousePos();
    mx -= Bounds[0].x;
    my -= Bounds[0].y;
    
    Height = Bounds[1].y - Bounds[0].y;
    Width  = Bounds[1].x - Bounds[0].x;
    
    my = Height - my;
    
    MousePosX = (int32_t)mx;
    MousePosY = (int32_t)my;
}

/// Update the mouse position in view port
void Viewport::UpdateBound() {
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 minBound   = ImGui::GetWindowPos();
    
    minBound.x += CursorPos.x;
    minBound.y += CursorPos.y;
    
    ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
    Bounds[0] = { minBound.x, minBound.y };
    Bounds[1] = { maxBound.x, maxBound.y };
}
