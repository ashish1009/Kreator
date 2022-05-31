//
//  Viewport.cpp
//  iKan
//
//  Created by iKan on 31/05/22.
//

#include "Viewport.hpp"
#include "Scene/Entity.hpp"
#include "Editor/PropertyGrid.hpp"

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

/// render Imgui
void Viewport::RenderImgui() {
    // TODO: Make column widht size acc to font of Imgui
    ImGui::Begin("Viewport Data",nullptr, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushID("Viewport Data");

    ImGui::SetNextWindowContentSize(ImVec2(585.0f, 0.0f));
    ImGui::BeginChild("##Renderer Version", ImVec2(0, ImGui::GetFontSize() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Columns(6);
    ImGui::SetColumnWidth(0, 85);  ImGui::Text("%d x %d", MousePosX, MousePosY); ImGui::NextColumn();
    
    ImGui::SetColumnWidth(1, 80);  ImGui::Text("Focused : %d", Focused); ImGui::NextColumn();
    ImGui::SetColumnWidth(2, 80);  ImGui::Text("Hovered : %d", Hovered); ImGui::NextColumn();
    ImGui::SetColumnWidth(3, 80);  ImGui::Text("%d x %d", (int32_t)Size.x, (int32_t)Size.y); ImGui::NextColumn();
    
    if (HoveredEntity) {
        std::string entityName = HoveredEntity->GetComponent<TagComponent>().Tag;
        ImGui::SetColumnWidth(4, 60);  ImGui::Text("ID : %d", (uint32_t)(*HoveredEntity.get())); ImGui::NextColumn();
        ImGui::SetColumnWidth(5, 200);  ImGui::Text("Name : %s ", entityName.c_str()); ImGui::NextColumn();
    }
    else {
        ImGui::SetColumnWidth(4, 50);  PropertyGrid::HelpMarker("Hovered Entity ID to be shows here");     ImGui::NextColumn();
        ImGui::SetColumnWidth(5, 50);  PropertyGrid::HelpMarker("Hovered Entity Name to be shows here");   ImGui::NextColumn();
    }
    
    ImGui::EndChild();
    ImGui::PopID();
    ImGui::End(); // Viewport
}
