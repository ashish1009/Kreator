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
        static void DestroyEntities(const std::shared_ptr<Scene>& scene);
        static void RenderText(const glm::mat4& projection);
        static void Update(const glm::mat4& projection, float yPos);
        
    private:
        static void Init();
        
        // Texture to store tile sprite sheet
        static std::shared_ptr<Texture> s_Sprite;
        static std::shared_ptr<Texture> s_SelectePlayer;
        
        // Map of subtexture to character tile
        static std::unordered_map<char, std::shared_ptr<SubTexture>> s_TileMap;
        
        static std::vector<std::shared_ptr<Entity>> s_Entities;
    };
    
}
