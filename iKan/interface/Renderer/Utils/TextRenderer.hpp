//
//  TextRendering.hpp
//  iKan
//
//  Created by iKan on 11/06/22.
//

#pragma once

// Text Renderer APIs
// Responsible for
// - Initialize the Text rednder
// - User friendly API to renderant string

namespace iKan {
    
    /// Wrapper class for storing Text Renderer API
    class TextRenderer {
    public:
        /// Initialze the Text renderer. Create All buffers needed to store Data (Both Renderer and CPU)
        static void Init();
        /// Shutdown on Text the Batch Renderer. Delete all the allocated Data
        static void Shutdown();
        
        /// Begin the Batch renderer Scene
        /// @param viewProj View projection matrix of Camera
        static void BeginBatch(const glm::mat4& cameraViewProj);
        /// End the current batch by rendering all the vertex
        static void EndBatch();

        /// Render the Text in Window
        /// @param text Text to be rendereed
        /// @param x X Position of Text
        /// @param y y Position of Text
        /// @param scale Size of text
        /// @param color Color of text
        static void RenderText(std::string text, const glm::mat4& transform, float x, float y, float scale, glm::vec3 color, uint32_t entID);
        
    private:
        // Making Singleton
        TextRenderer() = default;
        MAKE_SINGLETON(TextRenderer);
        
        // Member Methods
        /// Initialze the Quad Renderer Data
        static void Flush();
        static void NextBatch();
    };
    
}
