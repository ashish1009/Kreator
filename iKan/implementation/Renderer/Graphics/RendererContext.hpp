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
        
        /// Initialize the Rendeer Context
        virtual void Init() = 0;
        
        /// Swap the Buffer each frame
        virtual void SwapBuffers() = 0;
        
        static std::unique_ptr<RendererContext> Create(GLFWwindow* window);
    };
    
}
