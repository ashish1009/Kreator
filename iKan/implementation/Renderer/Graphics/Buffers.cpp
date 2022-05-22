//
//  Buffer.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "Buffers.hpp"
#include "Platform/OpenGL/OpenGLRendererBuffer.hpp"

using namespace iKan;

/// Create Instance of vertex Buffer based on the renderer API
/// @param data Data pointer to be stored in Vertex buffer
/// @param size size of data to be stored in buffer
std::shared_ptr<VertexBuffer> VertexBuffer::Create(void *data, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(data, size);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}

/// Create Instance of vertex Buffer based on the renderer API
/// @param size size of data to be stored in buffer
std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}

/// Creating instance of Index Buffer
/// @param count Number of Indices
/// @param data Data to be stored in Index Buffer
std::shared_ptr<IndexBuffer> IndexBuffer::CreateWithCount(void* data, uint32_t count) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL : return std::make_shared<OpenGLIndexBuffer>(data, count * (uint32_t)sizeof(uint32_t));
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}

/// Creating instance of Index Buffer
/// @param count Size of data
/// @param data Data to be stored in Index Buffer
std::shared_ptr<IndexBuffer> IndexBuffer::CreateWithSize(void* data, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL : return std::make_shared<OpenGLIndexBuffer>(data, size);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}

/// Get the number of bytes in Shader data types. Size
/// @param type Shader data type
static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::NoType : return 0;
        case ShaderDataType::Float  : return 4;
        case ShaderDataType::Float2 : return 4 * 2;
        case ShaderDataType::Float3 : return 4 * 3;
        case ShaderDataType::Float4 : return 4 * 4;
        case ShaderDataType::Mat3   : return 4 * 3 * 3;
        case ShaderDataType::Mat4   : return 4 * 4 * 5;
        case ShaderDataType::Int    : return 4;
        case ShaderDataType::Int2   : return 4 * 2;
        case ShaderDataType::Int3   : return 4 * 3;
        case ShaderDataType::Int4   : return 4 * 4;
        case ShaderDataType::Bool   : return 1;
    }
    IK_CORE_ASSERT(false, "Invalid Type");
    return 0;
}

/// Get the number of same data type elements. Count
/// @param type Shader data type
static uint32_t GetElementCount(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::NoType : return 0;
        case ShaderDataType::Float  : return 1;
        case ShaderDataType::Float2 : return 2;
        case ShaderDataType::Float3 : return 3;
        case ShaderDataType::Float4 : return 4;
        case ShaderDataType::Mat3   : return 3;
        case ShaderDataType::Mat4   : return 4;
        case ShaderDataType::Int    : return 1;
        case ShaderDataType::Int2   : return 2;
        case ShaderDataType::Int3   : return 3;
        case ShaderDataType::Int4   : return 4;
        case ShaderDataType::Bool   : return 1;
    }
    IK_CORE_ASSERT(false, "Unkown Shader datatype!! ");
}

/// Buffer Element Constructor
/// @param name Name of Buffer element
/// @param type type of element
/// @param normalized is Normalised
BufferElement::BufferElement(const std::string& name, ShaderDataType type, bool normalized)
: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Count(GetElementCount(type)), Offset(0), Normalized(normalized) {}

/// Buffer Layout Constructor
/// @param elements initializer list for buffer layour
BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
: m_Elements(elements) {
    CalculateOffsetAndStride();
}

/// Calculate the offet for element and update the stride value
void BufferLayout::CalculateOffsetAndStride() {
    size_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements) {
        element.Offset = offset;
        offset += element.Size;
        m_Stride += element.Size;
    }
}

/// Get all the buffer elements
const std::vector<BufferElement> BufferLayout::GetElements() const {
    return m_Elements;
}

/// Get the stride value fo buffer element
uint32_t BufferLayout::GetStride() const {
    return m_Stride;
}

/// Begin iterator
std::vector<BufferElement>::iterator BufferLayout::begin() {
    return m_Elements.begin();
}

/// End iterator
std::vector<BufferElement>::iterator BufferLayout::end() {
    return m_Elements.end();
}

/// const begin itereator
std::vector<BufferElement>::const_iterator BufferLayout::begin() const {
    return m_Elements.begin();
}

/// const end iteratoe
std::vector<BufferElement>::const_iterator BufferLayout::end() const {
    return m_Elements.end();
}
