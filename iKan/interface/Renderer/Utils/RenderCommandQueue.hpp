//
//  RendererCommandQueue.hpp
//  iKan
//
//  Created by iKan on 22/06/22.
//

#pragma once

namespace iKan {

    class RenderCommandQueue {
    public:
        /// Fucntion pointer type def
        typedef void(*RenderCommandFn)(void*);
        
        /// Renderer Command queue constructor
        RenderCommandQueue();
        /// Renderer Command queue destructor
        ~RenderCommandQueue();
        
        /// Allocate a funciton to renderer command queue
        /// @param func Fucntion pointer to be registeded
        /// @param size size of function
        void* Allocate(RenderCommandFn func, uint32_t size);
        /// Execute all the function in queue
        void Execute();

    private:
        uint8_t* m_CommandBuffer;
        uint8_t* m_CommandBufferPtr;
        uint32_t m_CommandCount = 0;
    };

}
