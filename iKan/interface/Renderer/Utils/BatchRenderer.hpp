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
        static void Init();
        static void Shutdown();

    private:
        BatchRenderer() = default;
    };
    
}