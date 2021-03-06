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
        /// Load the font for freetype
        /// @param fontPath path of font
        /// NOTE: Load the Font before using any other API In clientw
        static void LoadFreetype(const std::string& fontPath);

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
        /// @param transform Transform of COlor
        /// @param color Color of text
        /// @param entID Pixel ID of Text
        static void RenderText(std::string text, const glm::mat4& transform, const glm::vec4& color, int32_t entID);

        /// Render the Text in Window
        /// @param text Text to be rendereed
        /// @param position Text Poistion
        /// @param scale Text Poistion
        /// @param color Color of text
        static void RenderText(std::string text, glm::vec3 position, const glm::vec2& scale, const glm::vec4& color);
            
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
