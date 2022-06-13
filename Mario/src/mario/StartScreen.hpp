//
//  StartScreen.hpp
//  Mario
//
//  Created by iKan on 13/06/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Mario {

    /// Background Wrapper to store Entity to render background
    class StartScreen {
    public:
        static void CreateEntities(const std::shared_ptr<Scene>& scene);
        static void RenderText(const glm::mat4& projection);
        
    private:
        static void Init();
        
        // Texture to store tile sprite sheet
        static std::shared_ptr<Texture> s_Sprite;
        
        // Map of subtexture to character tile
        static std::unordered_map<char, std::shared_ptr<SubTexture>> s_TileMap;
    };
    
}
