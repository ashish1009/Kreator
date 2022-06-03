//
//  Header.h
//  Chess
//
//  Created by iKan on 24/05/22.
//

#pragma once

#include <iKanHeaders.h>
#include "ChessLayer.hpp"

using namespace iKan;

namespace Chess {

    /// Application for rendering all the layer. Derived from Core iKan::Application
    class ChessApp : public Application {
    public:
        /// Chess Application instance
        /// @param spec Core application specification
        ChessApp(const Application::Specification& spec);
        /// Chess Application Destructor
        virtual ~ChessApp();
        
    private:
        std::shared_ptr<Layer> m_RendererLayer;
    };
    
}
