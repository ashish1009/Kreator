//
//  TextRender.hpp
//  Mario
//
//  Created by iKan on 12/06/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Mario {
    
    struct TextRenderData {
        uint32_t Score;
        uint32_t CoinCount;
        
        uint32_t WorldLevel;
        uint32_t WorldSubLevel;
        
        uint32_t TimeLeft;
    };

    // Render all the text needed for Mario Game inclcude score and levels
    class TextRender {
    public:
        static void Init(const TextRenderData& textData);
        static void Update(const glm::mat4& projection, const glm::vec2& windowSize);
        
    private:
        TextRender() = default;
        MAKE_SINGLETON(TextRender);
    };
    
}
