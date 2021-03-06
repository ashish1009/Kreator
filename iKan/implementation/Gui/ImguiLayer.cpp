//
//  ImguiLayer.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "ImguiLayer.hpp"
#include "Core/Application.hpp"
#include "Core/Event/Event.h"

#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <ImGuizmo.h>

using namespace iKan;

/// Imgui layer COnstructor
ImguiLayer::ImguiLayer() : Layer("Imgui Layer") {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Imgui Layer instance ...");
}

/// Imgui Layer Desctructor
ImguiLayer::~ImguiLayer() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Imgui Layer instance !!!");
}

/// Attach ImGuiLayer
void ImguiLayer::Attach() {
    IK_CORE_INFO("{0} is attached to application", GetName());
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    const Application& app = Application::Get();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
    
    /* Setup Platform/Renderer bindings */
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    
    Renderer::Submit([]() {
        ImGui_ImplOpenGL3_Init("#version 410");
    });
}

/// Override font
/// @param defaultFont Default font path
/// @param boldFont bold Font Path
void ImguiLayer::SetFont(const std::string &defaultFont, const std::string &boldFont) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Store the Bold font as well
    io.Fonts->AddFontFromFileTTF(boldFont.c_str(), 15.0f);
    
    // Default font is Regular
    io.FontDefault = io.Fonts->AddFontFromFileTTF(defaultFont.c_str(), 15.0f);
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Imgui Font changed");
    IK_CORE_INFO("    Regular Font Path : {0}", defaultFont.c_str());
    IK_CORE_INFO("    Bold Font Path    : {0}", boldFont.c_str());
}

/// Detatch ImGuiLayer
void ImguiLayer::Detach() {
    IK_CORE_WARN("{0} is detached to application", GetName());
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/// Event handler in ImGuiLayer
/// @param event Triggered Event Instacne
void ImguiLayer::EventHandler(Event& event) {
    if (m_BlockEvents) {
        ImGuiIO& io = ImGui::GetIO();
        
        event.Handled |= event.IsInCategory(Event::EventCategoryMouse) & io.WantCaptureMouse;
        event.Handled |= event.IsInCategory(Event::EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
}

/// Begin ImGuiLayer
void ImguiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

/// End Imgui Layer
void ImguiLayer::End() {
    ImGuiIO& io      = ImGui::GetIO();
    const Application& app = Application::Get();
    io.DisplaySize   = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

const std::string& ImguiLayer::GetName() const { return m_Name; }

// Unused Functions in Imgui for now
void ImguiLayer::Update(Timestep ts) { }
void ImguiLayer::RenderGui() {}
