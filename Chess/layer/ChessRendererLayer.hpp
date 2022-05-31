//
//  RendererLayer.hpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Chess {
    
    /// Type of Block position
    typedef int8_t BLOCK_POSITION;
    
    // Constants
    static constexpr uint8_t MAX_ROWS = 8;
    static constexpr uint8_t MAX_COLUMNS = 8;
    
    /// Block Data
    struct Block {
        /// Position of block relative to Row and Column
        /// X{ 0 : 7} | Y{ 0 : 7 }
        BLOCK_POSITION X, Y;
        
        /// Block Index. e.g. Block at [2][3] is 2 * 8 + 3 = 19
        int8_t BlockIndex = -1;
        
        /// Entity of Each block
        std::shared_ptr<Entity> Entity;
    };
    
    /// Chess Renderer Layer
    class ChessRendererLayer : public Layer {
    public:
        ChessRendererLayer();
        virtual ~ChessRendererLayer();
        
        void Attach() override;
        void Update(Timestep ts) override;
        void RenderGui() override;
        void Detach() override;
        void EventHandler(Event&) override;
        
        const std::string& GetName() const override { return m_Name; }
        
    private:
        // Member functions
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        
        void UpdateHoveredEntity();
        
        // Member variabls
        std::shared_ptr<Scene> m_Scene;
        
        // Stores the blocks data of chess
        Block m_Blocks[MAX_ROWS][MAX_COLUMNS];
        
        // Camera Entity
        std::shared_ptr<Entity> m_CameraEntity;
        
        // Viewport Data
        Viewport m_ViewportData;
    };

}
