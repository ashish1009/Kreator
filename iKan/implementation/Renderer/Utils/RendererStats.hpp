//
//  RendererStats.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Editor/PropertyGrid.hpp"

namespace iKan {
    
    /// Stores the statistics of renderer
    struct RendererStatistics {
        uint32_t VertexBufferSize = 0, IndexBufferSize = 0, TextureBufferSize = 0;
        
        uint32_t DrawCalls    = 0;
        uint32_t VertexCount  = 0;
        uint32_t IndexCount   = 0;
        
        /// Reset Few stat each frame
        void ResetEachFrame() {
            DrawCalls = 0;
            IndexCount = 0;
            VertexCount = 0;
        }
        
        /// Reset all stats
        void ResetAll() {
            VertexBufferSize = 0;
            IndexBufferSize = 0;
            TextureBufferSize = 0;
            
            DrawCalls = 0;
            VertexCount = 0;
            IndexCount = 0;
        }
        
        // Return the static instance of RendererStatistics
        static RendererStatistics& Get() {
            static RendererStatistics s_Instance;
            return s_Instance;
        }
        
        /// RendererStats
        /// @param pIsOpen flag to check presence
        void RendererStats() {
            ImGui::Begin("Renderer Stats", nullptr, ImGuiWindowFlags_NoScrollbar);
            ImGui::PushID("Renderer Stats");
            
            ImGui::SetNextWindowContentSize(ImVec2(715.0f, 0.0f));
            ImGui::BeginChild("##Renderer Stats", ImVec2(0, ImGui::GetFontSize() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::Columns(6);
            
            ImGui::SetColumnWidth(0, 65); ImGui::Text("%d", DrawCalls); PropertyGrid::HelpMarker("Draw Calls"); ImGui::NextColumn();
            ImGui::SetColumnWidth(1, 70); ImGui::Text("%d", VertexCount); PropertyGrid::HelpMarker("Vertex Counts"); ImGui::NextColumn();
            ImGui::SetColumnWidth(2, 70); ImGui::Text("%d", VertexCount); PropertyGrid::HelpMarker("Index Counts"); ImGui::NextColumn();
            
            ImGui::SetColumnWidth(3, 170); ImGui::Text("%.2f KB (%.2f MB)", (float)VertexBufferSize / (float)1000.0f, (float)VertexBufferSize / (float)1000000.0f); PropertyGrid::HelpMarker("Vertex Buffer Size"); ImGui::NextColumn();
            ImGui::SetColumnWidth(4, 170); ImGui::Text("%.2f KB (%.2f MB)", (float)IndexBufferSize / (float)1000.0f, (float)IndexBufferSize / (float)1000000.0f); PropertyGrid::HelpMarker("Index Buffer Size"); ImGui::NextColumn();
            ImGui::SetColumnWidth(5, 170); ImGui::Text("%.2f KB (%.2f MB)", (float)TextureBufferSize / (float)1000.0f, (float)TextureBufferSize / (float)1000000.0f); PropertyGrid::HelpMarker("Texture Buffer Size"); ImGui::NextColumn();
            
            ImGui::Columns(1);
            ImGui::EndChild();
            
            ImGui::PopID();
            ImGui::End();
        }

    private:
        RendererStatistics() = default;
        MAKE_SINGLETON(RendererStatistics);
    };
    
}
