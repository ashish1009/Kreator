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
//        /// Holds all state information relevant to a character as loaded using FreeType
//        struct Character {
//            uint32_t TextureID;     // ID handle of the glyph texture
//            glm::ivec2 Size;        // Size of glyph
//            glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
//            uint32_t   Advance;     // Horizontal offset to advance to next glyph
//            
//            Character(uint32_t textureID, glm::ivec2 size, glm::ivec2 bearing, uint32_t advance )
//            : TextureID(textureID), Size(size), Bearing(bearing), Advance(advance) {}
//        };
        
        /// Initialze the Text renderer. Create All buffers needed to store Data (Both Renderer and CPU)
        static void Init();
        /// Shutdown on Text the Batch Renderer. Delete all the allocated Data
        static void Shutdown();
        
        /// Begin the Batch renderer Scene
        /// @param viewProj View projection matrix of Camera
        static void BeginBatch(const glm::mat4& cameraViewProj);
        
        /// Render the Text in Window
        /// @param text Text to be rendereed
        /// @param x X Position of Text
        /// @param y y Position of Text
        /// @param scale Size of text
        /// @param color Color of text
        static void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

    };
    
}
