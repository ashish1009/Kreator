//
//  Shader.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

namespace iKan {
    
    /// Maximum Texture slot supported by Senderer Shader
    /// Current Open GL Version 4.1 supports only 16 Texture slot in Shader
    /// TODO: Change this value based on the Open GL Version
    static constexpr uint32_t MaxTextureSlotsInShader = 16;

    /// Interface class for Storing Renderer Shader Compiler. Implementation is depending on the
    /// Supported Renerer API.
    class Shader {
    public:
    };
    
}
