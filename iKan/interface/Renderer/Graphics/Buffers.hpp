//
//  Buffer.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    /// Data types of Shader we are going to use
    enum class ShaderDataType : uint8_t {
        NoType = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };
    
    /// Stores all the property of a element in a buffer
    struct BufferElement {
        std::string Name = "";
        ShaderDataType Type;
        uint32_t Size = 0;
        uint32_t Count = 0;
        size_t Offset = 0;
        bool Normalized = false;
        
        BufferElement(const std::string& name, ShaderDataType type, bool normalized = false);
    };
    
    /// Layout representation of Vertex Data to be stored and used by Shader.
    /// NOTE: All API to be used by the implementation of Vertex Buffer
    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& elements);
        
        const std::vector<BufferElement> GetElements() const;
        
        uint32_t GetStride() const;
        
        std::vector<BufferElement>::iterator begin();
        std::vector<BufferElement>::iterator end();
        std::vector<BufferElement>::const_iterator begin() const;
        std::vector<BufferElement>::const_iterator end() const;
        
    private:
        // member function
        void CalculateOffsetAndStride();
        
        // Member variable
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };
    
    /// Interface class for Storing Renderer Vertex Buffer. Implementation is depending on the
    /// Supported Renerer API.
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        
        /// Bind the Vertex Buffer before rendering
        virtual void Bind() const = 0;
        /// Unbind the Vertex Buffer after rendering
        virtual void Unbind() const = 0;
        
        /// Load the Data in Vertex Buffer (GPU) at run time.
        /// For each draw call we can dynamically load the data.
        /// @param data Data pointer
        /// @param size size of Data
        virtual void SetData(void* data, uint32_t size) = 0;
        
        /// Store the Data Layout that need to be stored in Vertex Buffer
        /// This should be same layout as the Shader expect the Vertex Data
        /// @param layout Layout to be stored
        virtual void AddLayout(const BufferLayout& layout) = 0;
        
        /// Return the size of Vertex Buffer in GPU
        virtual uint32_t GetSize() const = 0;
        /// Return the Data Buffer stored in GPU
        virtual Buffer GetData() const = 0;
        /// Return the renderer ID of Vertex Buffer
        virtual RendererID GetRendererID() const = 0;
        /// Return the Buffer layout stored in Vertex Buffer
        virtual const BufferLayout& GetLayout() const = 0;

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
        
        /// Bind the Index Buffer before rendering
        virtual void Bind() const = 0;
        /// Unbind the Index Buffer after rendering
        virtual void Unbind() const = 0;

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
