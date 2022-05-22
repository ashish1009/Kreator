//
//  OpenGLRendererContext.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include <glad/glad.h>
#include "OpenGLRendererContext.hpp"

using namespace iKan;

/// Constructor Open GL Renderer Context
/// @param window GLFW Window instance
OpenGLRendererContext::OpenGLRendererContext(GLFWwindow* window) : m_Window(window) {
    IK_CORE_ASSERT(window, "Window pointer is NULL !!!");
    IK_CORE_INFO("Creating Open GL Renderer Context ... ");
    Init();
}

/// Destructor Open GL Graphics Context
OpenGLRendererContext::~OpenGLRendererContext() {
    PROFILE();
    IK_CORE_WARN("Destroying Open GL Renderer Context !!! ");
}

/// Initialize the Open GL Graphics Context
void OpenGLRendererContext::Init() {
    PROFILE();
    
    // Create GLFW Window Context.
    // NOTE: This function makes the OpenGL or OpenGL ES context of the specified window
    //       current on the calling thread.  A context must only be made current on
    //       a single thread at a time and each thread can have only a single current
    //       context at a time.
    glfwMakeContextCurrent(m_Window);
    
    // We pass GLAD the function to load the address of the OpenGL function
    // pointers which is OS-specific. GLFW gives us glfwGetProcAddress that
    // defines the correct function based on which OS we're compiling for
    IK_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Can not initialize the Glad");
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("    Initialize Open GL Renderer context ");
    IK_CORE_INFO("    OpenGl Vendor Information :");
    IK_CORE_INFO("        Vendor   : {0} ", glGetString(GL_VENDOR));
    IK_CORE_INFO("        Renderer : {0} ", glGetString(GL_RENDERER));
    IK_CORE_INFO("        Version  : {0} ", glGetString(GL_VERSION));
    IK_LOG_SEPARATOR();
}

/// Swap buffer each frame
void OpenGLRendererContext::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}
