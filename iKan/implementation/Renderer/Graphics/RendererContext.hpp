//
//  RendererContext.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include <GLFW/glfw3.h>

namespace iKan {
    
    /// Interface for Renderer Context. Instance to be created based on the current API Supported
    class RendererContext {
    public:
        virtual ~RendererContext() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        
        // Static Methods
        static std::unique_ptr<RendererContext> Create(GLFWwindow* window);
    };
    
}
