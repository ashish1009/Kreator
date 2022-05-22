//
//  OpenGLRendererPipeline.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Graphics/Pipeline.hpp"

namespace iKan {
    
    class OpenGLRendererPipeline : public Pipeline {
    public:
        OpenGLRendererPipeline();
        virtual ~OpenGLRendererPipeline();
        
        void Bind() const override;
        void Unbind() const override;
        
        RendererID GetRendererID() const override { return m_RendererID; };
        
    private:
        RendererID m_RendererID = 0;
    };
    
}
