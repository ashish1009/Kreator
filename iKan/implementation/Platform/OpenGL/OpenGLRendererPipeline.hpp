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
        
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
        
        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer;; }

        RendererID GetRendererID() const override { return m_RendererID; };
        
    private:
        RendererID m_RendererID = 0;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
    
}
