//
//  PropertyGrid.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "PropertyGrid.hpp"

using namespace iKan;

/// Helper to display a little (?) mark which shows a tooltip when hovered. In your own code you may want to display an
/// actual icon if you are using a merged icon fonts (see docs/FONTS.txt)
/// @param desc Helper string
/// @param sameLine Check is it in same line
void PropertyGrid::HelpMarker(const char* desc, bool sameLine) {
    if (sameLine)
        ImGui::SameLine();
    
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
