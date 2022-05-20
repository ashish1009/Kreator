//
//  ImguiAPI.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "ImguiAPI.hpp"

using namespace iKan;

/// Enable the Imgui Window Docking
void ImguiAPI::StartDcocking() {
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = false;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;// | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}

/// Disable the Imgui Window Docking
void ImguiAPI::EndDcocking() {
    ImGui::End();
}

/// Render Frame buffer Imgui windw
void ImguiAPI::FrameRate() {
    ImGui::Begin("Frame Rate", nullptr, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushID("Frame Rate");

    ImGui::SetNextWindowContentSize(ImVec2(45, 0.0f));
    ImGui::BeginChild("##Frame Rate", ImVec2(0, ImGui::GetFontSize() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::EndChild();

    ImGui::PopID();
    ImGui::End();
}
