//
//  Buffer.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "Buffer.hpp"
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

