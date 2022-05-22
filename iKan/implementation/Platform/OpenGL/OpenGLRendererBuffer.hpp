//
//  OpenGLRendererBuffer.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Graphics/Buffers.hpp"

namespace iKan {
    
    /// Implemantation for creating the Opne GL Vertex Buffer
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(void* data, uint32_t size);
        OpenGLVertexBuffer(uint32_t size);
        virtual ~OpenGLVertexBuffer();
        
        void Bind() const override;
        void Unbind() const override;
        
        void SetData(void* data, uint32_t size) override;
        void AddLayout(const BufferLayout& layout) override { m_Layout = layout; }

        // Getters
        uint32_t GetSize() const override { return m_Size; }
        Buffer GetData() const override { return m_Data; }
        RendererID GetRendererID() const override { return m_RendererID; }
        const BufferLayout& GetLayout() const override { return m_Layout; }

    private:
        RendererID m_RendererID = 0;
        uint32_t m_Size = 0;
        Buffer m_Data;
        BufferLayout m_Layout;
    };
    
    /// Implemantation for creating the Opne GL Index Buffer
    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(void* data, uint32_t size);
        virtual ~OpenGLIndexBuffer();
        
        void Bind() const override;
        void Unbind() const override;

        // Getters
        uint32_t GetCount() const override { return m_Count; }
        uint32_t GetSize() const override { return m_Size; }
        Buffer GetData() const override { return m_Data; }
        RendererID GetRendererID() const override { return m_RendererID; }
        
    private:
        RendererID m_RendererID = 0;
        uint32_t m_Size = 0;
        uint32_t m_Count = 0;
        Buffer m_Data;
    };
    
}
