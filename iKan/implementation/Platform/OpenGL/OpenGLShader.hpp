//
//  OpenGLShader.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Graphics/Shader.hpp"
#include <glad/glad.h>

namespace iKan {

    /// Implementation for Compiling Open GL Shader
    class OpenGLShader : public Shader {
    public:
        // Constants
        static constexpr uint32_t MaxShaderSupported = 3;

        // Member functions
        OpenGLShader(const std::string& filepath);
        virtual ~OpenGLShader();
        
        void Bind() const override;
        void Unbind() const override;
        
        const std::string& GetName() const override { return m_Name; }
        const std::string& GetFilePath() const override { return m_AssetPath; }
        RendererID GetRendererID() const override { return m_RendererID; }

    private:
        // Member functions
        void PreprocessFile(const std::string& source);
        void Compile();

        // Member variables
        RendererID m_RendererID = 0;
        std::string m_AssetPath = "", m_Name = "";
        std::unordered_map<GLenum, std::string> m_SourceShaderString;
    };
    
}
