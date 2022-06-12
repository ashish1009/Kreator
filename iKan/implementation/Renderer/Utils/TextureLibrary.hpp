//
//  TextureLibrary.hpp
//  iKan
//
//  Created by iKan on 01/06/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    class Texture;
    class Renderer;
    class TextureLibrary {
    private:
        TextureLibrary() = default;

        static std::shared_ptr<Texture> GetTexture(const std::string& path, bool minLinear = true, bool magLinear = true);
        static void ResetTextures();
        static std::unordered_map<std::string, std::shared_ptr<Texture>> s_TextureLibrary;
        
        // Renderer ID APIs
        static bool IsTextureRendererIDExist(RendererID rendererId);
        static void AddRendererIDs(RendererID rendererId);
        static void RemoveRendererIDs(RendererID rendererId);

        // Member Variables
        static std::set<RendererID> s_TextureRendererIDs;
        
        friend class Renderer;
    };
    
}
