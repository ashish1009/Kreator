//
//  TextRendering.hpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#pragma once

#include <iKanHeaders.h>
#include "mario/Common.hpp"

using namespace iKan;

namespace Mario {

    // Render all the text needed for Mario Game inclcude score and levels
    class TextRender {
    public:
        
        /// Just a wrapper of all Run time text.
        /// Each elemnt will be copied from somewhere
        struct Data {
            PlayerScore Score;
            PlayerLevel Level;
            uint32_t TimeLeft = 0;
            
            Data(const PlayerScore& score, const PlayerLevel& level, uint32_t timeLeft);
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
