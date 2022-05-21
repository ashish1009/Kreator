//
//  OpenGLRendererBuffer.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Graphics/Buffer.hpp"

namespace iKan {
    
    /// Implemantation for creating the Opne GL Vertex Buffer
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(void* data, uint32_t size);
        OpenGLVertexBuffer(uint32_t size);
        virtual ~OpenGLVertexBuffer();
    };
    
    /// Implemantation for creating the Opne GL Index Buffer
    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(void* data, uint32_t size);
        virtual ~OpenGLIndexBuffer();
    };
    
}
