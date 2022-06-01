//
//  TextureLibrary.hpp
//  iKan
//
//  Created by iKan on 01/06/22.
//

#pragma once

namespace iKan {
    
    class Texture;
    class Renderer;
    class TextureLibrary {
    private:
        TextureLibrary() = default;

        static std::shared_ptr<Texture> GetTexture(const std::string& path);
        static void ResetTextures();

        static std::unordered_map<std::string, std::shared_ptr<Texture>> s_TextureLibrary;
        
        friend class Renderer;
    };
    
}
