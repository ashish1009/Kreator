//
//  Application.cpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#include "Application.hpp"

using namespace iKan;

// Static Member variable Declaration and Initialisation
Application* Application::s_Instance = nullptr;

/// Copy Constructor Application Specification
/// @param other Specificaion reference
Application::Specification::Specification(const Application::Specification& other) {
    IK_CORE_INFO("Copying Application::Specification ... ");
    
    Name = other.Name;
    StartMaximized = other.StartMaximized;
    Resizable = other.Resizable;
    EnableGui = other.EnableGui;
    
    // NOTE: For now this copy constructor is only to Avoid Redundant copying the data
}

/// Applciation Constructor
/// @param spec Specification defined for application
Application::Application(const Specification& spec)
: m_Specification(spec) {
    // If instance already created then abort the application
    // as multiple instacne should not be therer
    IK_CORE_ASSERT(!s_Instance, "Application already exists !!!");
    s_Instance = this;

    IK_CORE_INFO("Creating Core Application Instance ...");
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("    Application Specifications are");
    IK_CORE_INFO("    Name                  : {0}", m_Specification.Name);
    IK_CORE_INFO("    Maximized at startup  : {0}", m_Specification.StartMaximized);
    IK_CORE_INFO("    Resizable             : {0}", m_Specification.Resizable);
    IK_CORE_INFO("    Enable GUI            : {0}", m_Specification.EnableGui);
    IK_LOG_SEPARATOR();
    
    Init();
}

/// Initialize the Application data
void Application::Init() {
    m_LayerStack = std::make_unique<LayerStack>();
}

/// Applciation Destructor
Application::~Application() {
    IK_CORE_WARN("Destroying Core Application Instance !!!");
}

/// Update the Application each frame
void Application::Run() {
    IK_CORE_INFO("------------------ Starting Game Loop ---------------------- ");

    // Updating all the attached layer
    for (auto& layer : *m_LayerStack.get())
        layer->Update();
    
    if (m_Specification.EnableGui)
        RenderGui();
    IK_CORE_INFO("------------------ Ending Game Loop ---------------------- ");
}

/// Handle the External event interuption in window
void Application::EventHandler() {
    
}

/// Render GUI Window
void Application::RenderGui() {
    // Render Imgui for all layers
    for (auto& layer : *m_LayerStack.get())
        layer->RenderGui();
}

// ----- Push and Pop Layer in staeck ----
void Application::PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack->PushLayer(layer); }
void Application::PopLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack->PopLayer(layer); }

// --------------- Getters --------------- 
/// Return the reference of Application Instance
const Application& Application::Get() { return *s_Instance; }
// ---------------------------------------
