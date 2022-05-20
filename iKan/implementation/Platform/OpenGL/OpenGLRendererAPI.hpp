//
//  OpenGLRendererAPI.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Renderer/Utils/RendererAPI.hpp"

namespace iKan {

    class OpenGLRendererAPI : public RendererAPI {
    public:
        OpenGLRendererAPI();
        virtual ~OpenGLRendererAPI();
        
        void Init() const override;
        void Shutdown() const override;
    };
    
}
