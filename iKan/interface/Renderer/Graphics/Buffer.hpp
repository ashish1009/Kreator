//
//  Buffer.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    /// Interface class for Storing Renderer Vertex Buffer. Implementation is depending on the
    /// Supported Renerer API.
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        
        /// Return the size of Vertex Buffer in GPU
        virtual uint32_t GetSize() const = 0;
        /// Return the Data Buffer stored in GPU
        virtual Buffer GetData() const = 0;
        /// Return the renderer ID of Vertex Buffer
        virtual RendererID GetRendererID() const = 0;

        /// Static API to create instance of Vertex Buffer
        /// @param data Data pointer to be stored in GPU
        /// @param size size of data
        static std::shared_ptr<VertexBuffer> Create(void* data, uint32_t size);
        /// Static API to create instance of Vertex Buffer
        /// @param size size of data
        static std::shared_ptr<VertexBuffer> Create(uint32_t size);
    };

    /// Interface class for Storing Renderer Index Buffer. Implementation is depending on the
    /// Supported Renerer API.
    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        /// Return the Number of Indices used by this Index Buffer
        virtual uint32_t GetCount() const = 0;
        /// Return the size of Index Buffer in GPU
        virtual uint32_t GetSize() const = 0;
        /// Return the Data Buffer stored in GPU
        virtual Buffer GetData() const = 0;
        /// Return the renderer ID of Index Buffer
        virtual RendererID GetRendererID() const = 0;

        /// Static API to Create instance of Index Buffer with count
        /// @param data Data pointer to be stored in GPU
        /// @param count count of indiced (NOTE: Single indices is taken as uint32_y)
        static std::shared_ptr<IndexBuffer> CreateWithCount(void* data, uint32_t count);
        /// Static API to Create instance of Index Buffer with size
        /// @param data Data pointer to be stored in GPU
        /// @param size size of index buffer
        static std::shared_ptr<IndexBuffer> CreateWithSize(void* data, uint32_t size);
    };
    
}
