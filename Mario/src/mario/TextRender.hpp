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

    // Render all the text needed for Mario Game inclcude score and levels
    class TextRender {
    public:
        static void Init(uint32_t worldLevel, uint32_t worldSubLevel, uint32_t timeLeft);
        static void Update(const glm::mat4& projection, const glm::vec2& windowSize);
        
    private:
        TextRender() = default;
        MAKE_SINGLETON(TextRender);
    };
    
}
