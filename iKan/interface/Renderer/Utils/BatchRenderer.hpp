//
//  BatchRenderer.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

// Batch Renderer APIs
// Responsible for
// - Initialize all Renderer buffer and store them for rendering 1 Batch in single draw call
// - User friendly API to render any 2D Shape (For now Quad and Cirlce)

namespace iKan {
    
    /// Wrapper class for storing Batch Renderer API
    class BatchRenderer {
    public:
        /// Initialze the Batch renderer. Create All buffers needed to store Data (Both Renderer and CPU)
        static void Init();
        /// Shutdown on destroy the Batch Renderer. Delete all the allocated Data
        static void Shutdown();

    private:
        // Making Singleton
        BatchRenderer() = default;
        MAKE_SINGLETON(BatchRenderer);
        
        // Member Methods
        /// Initialze the Quad Renderer Data
        static void InitQuadData();
    };
    
}
