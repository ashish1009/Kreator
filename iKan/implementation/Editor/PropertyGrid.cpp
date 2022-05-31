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
/// @param iconChar Icon to be printed as Help Marker (By default (?) will be renderes)
/// @param sameLine Check is it in same line
void PropertyGrid::HelpMarker(const char* desc, const char* iconChar, bool sameLine) {
    if (sameLine)
        ImGui::SameLine();
    
    ImGui::TextDisabled(iconChar);
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

/// Create Imgae in current Imgui window
/// @param textureID Texture id of Image to be rendered
/// @param size size of image need to be rendered inside Imgui window (this size will be visible as texture)
/// @param uv0 Left
/// @param uv1 Right
void PropertyGrid::Image(void* textureID, const glm::vec2& size, const glm::vec2& uv0, const glm::vec2& uv1) {
    ImGui::Image((void*)textureID, ImVec2{ size.x, size.y }, ImVec2{ uv0.x, uv0.y }, ImVec2{ uv1.x, uv1.y });
}
