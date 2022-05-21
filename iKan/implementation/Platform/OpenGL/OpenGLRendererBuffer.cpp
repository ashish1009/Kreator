//
//  OpenGLRendererBuffer.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLRendererBuffer.hpp"

using namespace iKan;

/// Open GL vertex Buffer Constructor
/// @param data Data pointer to be stored in Vertex buffer
/// @param size size of data to be stored in buffer
OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size) {
    PROFILE();
    IK_CORE_INFO("Creating Open GL Vertex Buffer with data of size {0} Bytes ({1} KB, {2} MB) ...", size, size / 1000, size / 1000000);
}

/// Open GL vertex Buffer Constructor with size only
/// @param size size of data to be stored in buffer
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
    PROFILE();
    IK_CORE_INFO("Creating Open GL Vertex Buffer without data of size {0} Bytes ({1} KB, {2} MB) ...", size, size / 1000, size / 1000000);
}

/// Open GL Vertex Buffer Detructor
OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    PROFILE();
    IK_CORE_WARN("Destroying Open GL Vertex Buffer !!!");
}

/// Open GL Index Buffer Constructor
/// @param size size of data to be stored
/// @param data data pointer
OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t size) {
    PROFILE();
    IK_CORE_INFO("Creating Open GL Index Buffer with size {0} Bytes ({1} KB, {2} MB) ...", size, size / 1000, size / 1000000);
}

/// Open GL Index Buffer Destructor
OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    PROFILE();
    IK_CORE_WARN("Destroying Open GL Index Buffer !!!");
}

