//
//  MacWindow.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "MacWindow.hpp"

using namespace iKan;

/// MAC Window Constructor
/// @param spec Window Spcification
MacWindow::MacWindow(const Window::Specification& spec) {
    m_Data.Specification = spec;
    IK_CORE_INFO("Creating MAC OS Window : {0}...", m_Data.Specification.Title);
    IK_CORE_INFO("    MAC Wondow Specifications are");
    IK_CORE_INFO("    Title      : {0}", m_Data.Specification.Title);
    IK_CORE_INFO("    Width      : {0}", m_Data.Specification.Width);
    IK_CORE_INFO("    Height     : {0}", m_Data.Specification.Height);
    IK_CORE_INFO("    VSync      : {0}", m_Data.Specification.VSync);
    IK_CORE_INFO("    Decorated  : {0}", m_Data.Specification.Decorated);
    IK_CORE_INFO("    FullScreen : {0}", m_Data.Specification.Fullscreen);
    IK_LOG_SEPARATOR();
    
    Init();
}

/// Initialize the Mac Window
void MacWindow::Init() {
    
}

/// Mac Window Destructor
MacWindow::~MacWindow() {
    IK_CORE_WARN("Destroying MAC OS Window: '{0}' ({1} x {2}) !!!", m_Data.Specification.Title, m_Data.Specification.Width, m_Data.Specification.Height);
}

/// Restisters the functions that will be called then GLFW interrupt triggers last argument of each function calls in
/// this funciton it the lamda that will be called when interrupt is triggered. So function arugument need to be u
void MacWindow::SetEventCallbacks() {
}

/// Shutdown the current Window
void MacWindow::Shutdown() {
    IK_CORE_WARN("Shutting down the Window MAC OS: {0} ({1} x {2})", m_Data.Specification.Title, m_Data.Specification.Width, m_Data.Specification.Height);
}

/// Set the VSynch
/// @param enabled New vsync flag
void MacWindow::SetVSync(bool enabled) {
}

/// Update Window Each Frame. To be called by application on each frame. It swap the context buffers
void MacWindow::Update() {
}

/// Make Window resizable if 'resizable' is true else Window can not be resized after callinh this API
/// @param resizable Resizable flag
void MacWindow::SetResizable(bool resizable) const {
}

/// Maximise the window. Call when Window need to be maximised
void MacWindow::Maximize() {
}

/// Place window at the center of the screen
void MacWindow::CenterWindow() {
}

/// Chnage the title of window
/// @param title New title of window
void MacWindow::SetTitle(const std::string& title) {
}

