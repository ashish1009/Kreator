//
//  MacWindow.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "MacWindow.hpp"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/ApplicationEvent.h"

using namespace iKan;

/// MAC Window Constructor
/// @param spec Window Spcification
MacWindow::MacWindow(const Window::Specification& spec) {
    PROFILE();
    m_Data.Specification = spec;

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating MAC OS Window ...");
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
    // Initialize the library
    IK_ASSERT(GLFW_TRUE == glfwInit(), "Can not Initialize GLFW Window");
    
    // Configure GLFW Context Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MAC OS
    
    if (!m_Data.Specification.Decorated) {
        // This removes titlebar on all platforms
        // and all of the native window effects on non-Windows platforms
        glfwWindowHint(GLFW_DECORATED, false);
    }

    if (m_Data.Specification.Fullscreen) {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        m_Window = glfwCreateWindow(mode->width, mode->height, m_Data.Specification.Title.c_str(), primaryMonitor, nullptr);
    }
    else {
        m_Window = glfwCreateWindow(m_Data.Specification.Width, m_Data.Specification.Height, m_Data.Specification.Title.c_str(), nullptr, nullptr);
    }
    
    if (m_Window == NULL) {
        glfwTerminate();
        IK_CORE_ASSERT(false, "Unable to create Window!!!!");
    }
    
    // Setting VSync as True
    glfwSwapInterval(m_Data.Specification.VSync);
    
    // Set the User defined pointer to GLFW Window, this pointer will be retrieved when
    // an interrupt will be triggered
    glfwSetWindowUserPointer(m_Window, &m_Data);
    
    // Set GLFW Callbacks
    SetEventCallbacks();

    // Creating Context according to API
    m_Context = RendererContext::Create(m_Window);
}

/// Mac Window Destructor
MacWindow::~MacWindow() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying MAC OS Window !!!");
    Shutdown();
    IK_LOG_SEPARATOR();
}

/// Restisters the functions that will be called then GLFW interrupt triggers last argument of each function calls in
/// this funciton it the lamda that will be called when interrupt is triggered. So function arugument need to be u
void MacWindow::SetEventCallbacks() {
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32_t width, int32_t height) {
        MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
        data.Specification.Width  = width;
        data.Specification.Height = height;
        
        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });
    
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });
    
    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int icontified) {
    });
    
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
        MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
        
        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(static_cast<KeyCode>(key));
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                data.EventCallback(event);
                break;
            }
        }
    });
    
    glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode){
        MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(static_cast<KeyCode>(keycode));
        data.EventCallback(event);
    });
    
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
        MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
        
        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(static_cast<MouseButton>(button));
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(static_cast<MouseButton>(button));
                data.EventCallback(event);
                break;
            }
        }
    });
    
    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event((MOUSE_POS_X_TYPE)xOffset, (MOUSE_POS_Y_TYPE)yOffset);
        data.EventCallback(event);
    });
    
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((MOUSE_POS_X_TYPE)xPos, (MOUSE_POS_Y_TYPE)yPos);
        data.EventCallback(event);
    });
}

/// Shutdown the current Window
void MacWindow::Shutdown() {
    IK_CORE_WARN("Shutting down the Window MAC OS");
    IK_CORE_WARN("    Title      : {0}", m_Data.Specification.Title);
    IK_CORE_WARN("    Width      : {0}", m_Data.Specification.Width);
    IK_CORE_WARN("    Height     : {0}", m_Data.Specification.Height);
    IK_CORE_WARN("    VSync      : {0}", m_Data.Specification.VSync);
    IK_CORE_WARN("    Decorated  : {0}", m_Data.Specification.Decorated);
    IK_CORE_WARN("    FullScreen : {0}", m_Data.Specification.Fullscreen);

    glfwTerminate();
    glfwDestroyWindow(m_Window);
}

/// Set the VSynch
/// @param enabled New vsync flag
void MacWindow::SetVSync(bool enabled) {
    IK_CORE_INFO("Setting VSynch : {0}", enabled);
    (true == enabled) ? glfwSwapInterval(1) : glfwSwapInterval(0);
    m_Data.Specification.VSync = enabled;
}

/// Update Window Each Frame. To be called by application on each frame. It swap the context buffers
void MacWindow::Update() {
    m_Context->SwapBuffers();
    glfwPollEvents();
    
    TIME currentFrame = glfwGetTime();
    m_Time            = currentFrame - m_LastFrameTime;
    m_LastFrameTime   = currentFrame;
}

/// Make Window resizable if 'resizable' is true else Window can not be resized after callinh this API
/// @param resizable Resizable flag
void MacWindow::SetResizable(bool resizable) const {
    glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
}

/// Maximise the window. Call when Window need to be maximised
void MacWindow::Maximize() {
    glfwMaximizeWindow(m_Window);
}

/// Place window at the center of the screen
void MacWindow::CenterWindow() {
    const GLFWvidmode* videmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int32_t x = (videmode->width / 2) - (m_Data.Specification.Width / 2);
    int32_t y = (videmode->height / 2) - (m_Data.Specification.Height / 2);
    glfwSetWindowPos(m_Window, x, y);
}

/// Chnage the title of window
/// @param title New title of window
void MacWindow::SetTitle(const std::string& title) {
    IK_CORE_INFO("New MAC Window Title is : {0} (NOTE: Window specificaiton instance in Application Spceification might have older Window name..)", title.c_str());
    m_Data.Specification.Title = title;
    glfwSetWindowTitle(m_Window, m_Data.Specification.Title.c_str());
}

