//
//  Application.cpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#include "Application.hpp"
#include "Core/LayerStack.hpp"
#include "Core/Event/ApplicationEvent.h"
#include "Renderer/Utils/Renderer.hpp"

using namespace iKan;

#ifdef IK_DEBUG_FEATURE

namespace Utils {
    /// Return the Operating System Type as string
    /// @param os OS Type in iKan Enum
    std::string GetOsNameAsString(OS os) {
        switch (os) {
            case OS::Mac : return "MAC OS";
            case OS::None :
            default:
                IK_ASSERT(false, "Invalid OS Type");
        }
    }
    
    /// Return the Renderer API Name as string
    /// @param api Renderer APi Name in iKan Enum
    std::string GetRendererAPINameAsString(Renderer::API api) {
        switch (api) {
            case Renderer::API::OpenGL : return "Open GL";
            case Renderer::API::None :
            default:
                IK_ASSERT(false, "Invalid Renderer API Type");
        }
    }
}

#endif


// Static Member variable Declaration and Initialisation
Application* Application::s_Instance = nullptr;

/// Copy Constructor Application Specification
/// @param other Specificaion reference
Application::Specification::Specification(const Application::Specification& other) {
    IK_CORE_TRACE("Copying Application::Specification  ");
    
    Name = other.Name;
    Os = other.Os;
    RendererAPI = other.RendererAPI;
    StartMaximized = other.StartMaximized;
    Resizable = other.Resizable;
    EnableGui = other.EnableGui;
    WindowSpec = other.WindowSpec;
    
    // NOTE: For now this copy constructor is only to Avoid Redundant copying the data
}

/// Copy Operator = for Application Specification
/// @param other Specificaion reference
Application::Specification& Application::Specification::operator=(const Application::Specification& other) {
    IK_CORE_TRACE("Copying (= operator) Application::Specification  ");
    
    Name = other.Name;
    Os = other.Os;
    RendererAPI = other.RendererAPI;
    StartMaximized = other.StartMaximized;
    Resizable = other.Resizable;
    EnableGui = other.EnableGui;
    WindowSpec = other.WindowSpec;
    
    // NOTE: For now this copy constructor is only to Avoid Redundant copying the data

    return *this;
}

/// Applciation Constructor
/// @param spec Specification defined for application
Application::Application(const Specification& spec)
: m_Specification(spec) {
    PROFILE();
    // If instance already created then abort the application
    // as multiple instacne should not be therer
    IK_CORE_ASSERT(!s_Instance, "Application already exists !!!");
    s_Instance = this;

    IK_CORE_INFO("Creating Core Application Instance ...");

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("    Application Specifications are");
    IK_CORE_INFO("        Name                          : {0}", m_Specification.Name);
    IK_CORE_INFO("        Operating System              : {0}", Utils::GetOsNameAsString(m_Specification.Os));
    IK_CORE_INFO("        Renderer API                  : {0}", Utils::GetRendererAPINameAsString(m_Specification.RendererAPI));
    IK_CORE_INFO("        Window Maximized at startup   : {0}", m_Specification.StartMaximized);
    IK_CORE_INFO("        Window Resizable              : {0}", m_Specification.Resizable);
    IK_CORE_INFO("        Enable GUI                    : {0}", m_Specification.EnableGui);
    IK_LOG_SEPARATOR();
    
    Init();
}

/// Initialize the Application data
void Application::Init() {
    m_LayerStack = std::make_unique<LayerStack>();
    
    Renderer::SetAPI(m_Specification.RendererAPI);
    
    // Create Window for application and register the Event callback function to Window
    m_Window = Window::Create(m_Specification.Os, m_Specification.WindowSpec);
    m_Window->SetEventFunction(IK_BIND_EVENT_FN(Application::EventHandler));
    
    if (m_Specification.StartMaximized)
        m_Window->Maximize();
    else
        m_Window->CenterWindow();
    
    m_Window->SetResizable(m_Specification.Resizable);
    
    Renderer::Init();
}

/// Applciation Destructor
Application::~Application() {
    PROFILE();
    IK_CORE_WARN("Destroying Core Application Instance !!!");
    Renderer::Shutdown();
}

/// Update the Application each frame
void Application::Run() {
    PROFILE();
    IK_CORE_INFO("------------------ Starting Game Loop ---------------------- ");
    
    while (m_IsRunning) {
        // Store the frame time difference
        m_Timestep = m_Window->GerTimestep();

        // Updating all the attached layer
        for (auto& layer : *m_LayerStack.get())
            layer->Update(m_Timestep);
        
        if (m_Specification.EnableGui)
            RenderGui();
        
        // Window update each frame
        m_Window->Update();
    }
    IK_CORE_INFO("------------------ Ending Game Loop ---------------------- ");
}

/// Handle the External event interuption in window
/// @param event Event (Base class) intance. Dispatch event from Event Dispatcher
void Application::EventHandler(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(IK_BIND_EVENT_FN(Application::WindowClose));
    
    // Event Handler for all layers
    for (auto& layer : *m_LayerStack.get())
        layer->EventHandler(event);
}

/// Trigger the Event when WIndow Closes
/// @param event Reference of Window Close event
bool Application::WindowClose(WindowCloseEvent& event) {
    Close();
    return false;
}

/// Render GUI Window
void Application::RenderGui() {
    // Render Imgui for all layers
    for (auto& layer : *m_LayerStack.get())
        layer->RenderGui();
}

/// Close the application
void Application::Close() {
    IK_CORE_INFO("Closing Core Application ");
    m_IsRunning = false;
}

// ----- Push and Pop Layer in staeck ----
void Application::PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack->PushLayer(layer); }
void Application::PopLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack->PopLayer(layer); }

// --------------- Getters ---------------
/// Return the GLFW Window native pointer
void* Application::GetWindowPtr() const { return (void*)m_Window->GetNativeWindow(); }
/// Return the iKan Window Instance reference
const Window& Application::GetWindow() const { return *m_Window; }

/// Return the reference of Application Instance
const Application& Application::Get() { return *s_Instance; }
// ---------------------------------------
