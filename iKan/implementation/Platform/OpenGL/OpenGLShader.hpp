//
//  OpenGLShader.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Graphics/Shader.hpp"

namespace iKan {

    /// Implementation for Compiling Open GL Shader
    class OpenGLShader : public Shader {
    public:
        // Member functions
        OpenGLShader(const std::string& filepath);
        virtual ~OpenGLShader();

    private:
        // Member variables
        RendererID m_RendererID = 0;
        std::string m_AssetPath = "", m_Name = "";
    };
    
}
