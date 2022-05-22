//
//  Pipeline.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"
#include "Renderer/Graphics/Buffers.hpp"

namespace iKan {
    
    /// Interface class for Storing Renderer Pipeline. Implementation is depending on the
    /// Supported Renerer API.
    class Pipeline {
    public:
        virtual ~Pipeline() = default;
        
        /// Bind the Pipeline before rendering
        virtual void Bind() const = 0;
        /// Unbind the Pipeline after rendering
        virtual void Unbind() const = 0;
        
        /// Add the Vertex Buffer inside Pipeline
        /// @param vertexBuffer Shared pointer of Vertex Buffer
        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        /// Update the current Index Buffer inside Pipeline
        /// @param indexBuffer Shared pointer of Index Buffer
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
        
        /// Return all the Vertex Buffer Stored in Pipeline
        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        /// Return the Current Index Buffer Stored in Pipeline
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

        /// Return the renderer ID of Pipeline
        virtual RendererID GetRendererID() const = 0;

        /// Static API to Create instance of Pipeline
        static std::shared_ptr<Pipeline> Create();
    };
    
}
