//
//  RendererCommandQueue.cpp
//  iKan
//
//  Created by iKan on 22/06/22.
//

#include "RenderCommandQueue.hpp"

using namespace iKan;

/// Render Command Queue constructor
RenderCommandQueue::RenderCommandQueue() {
    m_CommandBuffer    = new uint8_t[10 * 1024 * 1024]; // allocating 10mb buffer
    m_CommandBufferPtr = m_CommandBuffer;
    memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
}

/// Render Command Queue destructor
RenderCommandQueue::~RenderCommandQueue() {
    delete[] m_CommandBuffer;
}

/// Allocate commends to Render Command Queue
/// We are storing the address of Function and size of fucntion back to back like:
/// <FuncPtrA><SizeA><FunPtrB><SizeB>.....
/// @param fn Fucntion to be stored in queue
/// @param size size of function
void* RenderCommandQueue::Allocate(RenderCommandFn fn, uint32_t size) {
    // Store the function pointer in the buffer
    *(RenderCommandFn*)m_CommandBufferPtr = fn;

    // Move the pointer upto size of Function pointer size
    m_CommandBufferPtr += sizeof(RenderCommandFn);

    // Store the size of function from argument in the buffer
    *(uint32_t*)m_CommandBufferPtr = size;

    // move the pointer next to size.
    m_CommandBufferPtr += sizeof(uint32_t);

    void* memory = m_CommandBufferPtr;
    m_CommandBufferPtr += size;

    m_CommandCount++;
    return memory;
}

/// Execute Render Command from Queue
void RenderCommandQueue::Execute() {
    std::byte* buffer = (std::byte*)m_CommandBuffer;

    for (uint32_t i = 0; i < m_CommandCount; i++) {
        RenderCommandFn function = *(RenderCommandFn*)buffer;
        buffer += sizeof(RenderCommandFn);

        uint32_t size = *(uint32_t*)buffer;
        buffer += sizeof(uint32_t);
        function(buffer);
        buffer += size;
    }

    m_CommandBufferPtr = m_CommandBuffer;
    m_CommandCount = 0;
}
