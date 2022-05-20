//
//  OpenGLRendererContext.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Renderer/Graphics/RendererContext.hpp"

namespace iKan {
    
    /// Implementation of Renderer Graphics context for Open GL API
    class OpenGLRendererContext : public RendererContext {
    public:
        OpenGLRendererContext(GLFWwindow* window);
        virtual ~OpenGLRendererContext();
        
        void Init() override;
        void SwapBuffers() override;
        
    private:
        GLFWwindow* m_Window;
    };
    
}
