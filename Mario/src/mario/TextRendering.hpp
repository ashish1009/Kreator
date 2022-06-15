//
//  TextRendering.hpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Mario {

    // Render all the text needed for Mario Game inclcude score and levels
    class TextRender {
    public:
        
        /// Just a wrapper of all Run time text.
        /// Each elemnt will be copied from somewhere
        struct Data {
            uint32_t Score = 0;
            uint32_t CoinCount = 0;
            
            uint32_t WorldLevel = 0;
            uint32_t WorldSubLevel = 0;
            
            uint32_t TimeLeft = 0;
            
            Data(uint32_t score, uint32_t coinCount, uint32_t worldLevel, uint32_t wirldSubLevel, uint32_t timeLeft);
            Data() = default;
        };
        
        /// Render the text after game started
        /// @param projection Camera projection
        /// @param windowSize window Position
        static void UpdateRunTime(const glm::mat4& projection, const glm::vec2& windowSize, const Data& data);
        /// Render the text at init time before game started
        static void UpdateInitTime();
        
    private:
        TextRender() = default;
        MAKE_SINGLETON(TextRender);
    };
    
}
