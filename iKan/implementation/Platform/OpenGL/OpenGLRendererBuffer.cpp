//
//  OpenGLRendererBuffer.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLRendererBuffer.hpp"
#include "Renderer/Utils/RendererStats.hpp"
#include <glad/glad.h>

using namespace iKan;

/// Open GL vertex Buffer Constructor
/// @param data Data pointer to be stored in Vertex buffer
/// @param size size of data to be stored in buffer
OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size) : m_Size(size) {
    m_Data.Clear();
    m_Data = Buffer::Copy(data, m_Size);

    Renderer::Submit([this]() {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, m_Size, m_Data.Data, GL_STATIC_DRAW);
        
        IK_LOG_SEPARATOR();
        IK_CORE_INFO("Creating Open GL Vertex Buffer with Data...");
        IK_CORE_INFO("    Renderer ID : {0}", m_RendererID);
        IK_CORE_INFO("    Size        : {0} Bytes ({1} KBm {2} MB)", m_Size, m_Size / 1000, m_Size / 1000000);

        RendererStatistics::Get().VertexBufferSize += m_Size;
    });
}

/// Open GL vertex Buffer Constructor with size only
/// @param size size of data to be stored in buffer
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) : m_Size(size) {
    Renderer::Submit([this]() {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, m_Size, nullptr, GL_DYNAMIC_DRAW);

        IK_LOG_SEPARATOR();
        IK_CORE_INFO("Creating Open GL Vertex Buffer withoud Data ...");
        IK_CORE_INFO("    Renderer ID : {0}", m_RendererID);
        IK_CORE_INFO("    Size        : {0} Bytes ({1} KB, {2} MB)", m_Size, m_Size / 1000, m_Size / 1000000);

        RendererStatistics::Get().VertexBufferSize += m_Size;
    });
}

/// Open GL Vertex Buffer Detructor
OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    Renderer::Submit([this]() {
        IK_LOG_SEPARATOR();
        IK_CORE_WARN("Destroying Open GL Vertex Buffer !!!");
        IK_CORE_WARN("    Renderer ID : {0}", m_RendererID);
        IK_CORE_WARN("    Size        : {0} Bytes ({1} KB, {2} MB)", m_Size, m_Size / 1000, m_Size / 1000000);

        glDeleteBuffers(1, &m_RendererID);
    });
}

/// Open GL Vertex Buffer Bind function
void OpenGLVertexBuffer::Bind() const {
    Renderer::Submit([this]() {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    });
}

/// Open GL Vertex Buffer Unbind function
void OpenGLVertexBuffer::Unbind() const {
    Renderer::Submit([this]() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    });
}

/// Set the data to Open GL Vertex buffer dynamically
/// @param data Data pointer
/// @param size size of data
void OpenGLVertexBuffer::SetData(void* data, uint32_t size) {
    Renderer::Submit([this, size, data]() {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    });
}

/// Open GL Index Buffer Constructor
/// @param size size of data to be stored
/// @param data data pointer
OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t size) : m_Size(size), m_Count(m_Size / sizeof(uint32_t)) {
    m_Data.Clear();
    m_Data = Buffer::Copy(data, size);
    
    Renderer::Submit([this]() {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Size, m_Data.Data, GL_STATIC_DRAW);
        
        IK_LOG_SEPARATOR();
        IK_CORE_INFO("Creating Open GL Index Buffer ...");
        IK_CORE_INFO("    Renderer ID       : {0}", m_RendererID);
        IK_CORE_INFO("    Number of Indices : {0}", m_Count);
        IK_CORE_INFO("    Size              : {0} Bytes ({1} KB, {2} MB)", m_Size, m_Size / 1000, m_Size / 1000000);
        
        RendererStatistics::Get().IndexBufferSize += m_Size;
    });
}

/// Open GL Index Buffer Destructor
OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    Renderer::Submit([this]() {
        IK_LOG_SEPARATOR();
        IK_CORE_WARN("Destroying Open GL Index Buffer !!!");
        IK_CORE_WARN("    Renderer ID       : {0}", m_RendererID);
        IK_CORE_WARN("    Number of Indices : {0}", m_Count);
        IK_CORE_WARN("    Size              : {0} Bytes ({1} KB, {2} MB)", m_Size, m_Size / 1000, m_Size / 1000000);

        glDeleteBuffers(1, &m_RendererID);
    });
}

/// Open GL Index Buffer Binding
void OpenGLIndexBuffer::Bind() const {
    Renderer::Submit([this]() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    });
}

/// Unbind Open GL Buffer
void OpenGLIndexBuffer::Unbind() const {
    Renderer::Submit([this]() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    });
}
