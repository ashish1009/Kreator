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
    
    // Forward Declaration
    class Texture;
    class SubTexture;

    /// Wrapper class for storing Batch Renderer API
    class BatchRenderer {
    public:
        /// Initialze the Batch renderer. Create All buffers needed to store Data (Both Renderer and CPU)
        static void Init();
        /// Shutdown on destroy the Batch Renderer. Delete all the allocated Data
        static void Shutdown();
        
        /// Initialize the Batch for each Draw call (each Batch Rendering)
        /// @param cameraViewProj Ciew projection matrix of Camera
        static void BeginBatch(const glm::mat4& cameraViewProj);
        /// End the current batch by rendering all the vertex
        static void EndBatch();

        // ----------------------------------- Draw API -----------------------------------------------
        /// Draw Quad API with color
        /// @param transform Transformation matrix of Quad
        /// @param color Color of Quad
        /// @param entID Pixel ID of Quad
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t entID = -1 );
        /// Draw Quad API with Texture
        /// @param transform Transformation matrix of Quad
        /// @param texture Texture to be uploaded in Batch
        /// @param tilingFactor tiling factor of Texture (Scale by which texture to be Multiplied)
        /// @param tintColor Color of Quad
        /// @param entID Pixel ID of Quad
        static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tilingFactor = 1.0f, int32_t entID = -1 );
        
        /// Draw Quad API with sprite subtexture
        /// @param transform Transformation matrix of Quad
        /// @param subTexture Subtexture component
        /// @param entID entity ID of Quad
        static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<SubTexture>& subTexture, int32_t entID);
        
        /// Draw Circle API with color
        /// @param transform Transformation matrix of Circle
        /// @param color Color of Circle
        /// @param thickness thickneess of Circle
        /// @param fade fade of Circle Edge
        /// @param entID Pixel ID of Circle
        static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005, int32_t entID = -1);
        /// Draw Circle API with color
        /// @param transform Transformation matrix of Circle
        /// @param texture Texture to be uploaded in Batch
        /// @param tilingFactor tiling factor of Texture (Scale by which texture to be Multiplied)
        /// @param tintColor Color of Quad
        /// @param thickness thickneess of Circle
        /// @param fade fade of Circle Edge
        /// @param entID Pixel ID of Circle
        static void DrawCircle(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tilingFactor = 1.0f, float thickness = 1.0f, float fade = 0.005, int32_t entID = -1 );

    private:
        // Making Singleton
        BatchRenderer() = default;
        MAKE_SINGLETON(BatchRenderer);
        
        // Member Methods
        /// Initialze the Quad Renderer Data
        static void InitQuadData();
        static void InitCircleData();
        static void Flush();
        static void NextBatch();
        
        // Internal Helper API for Rendering Quad and Circle
        static void DrawTextureQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec2* textureCoords, float tilingFactor, const glm::vec4& tintColor, int32_t entID);
        static void DrawTextureCircle(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, float tilingFactor, const glm::vec4& color, float thickness, float fade, int32_t);
    };
    
}
