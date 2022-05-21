//
//  Buffer.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

namespace iKan {
    
    /// Interface class for Storing Renderer Vertex Buffer. Implementation is depending on the
    /// Supported Renerer API.
    class VertexBuffer {
    public:
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
