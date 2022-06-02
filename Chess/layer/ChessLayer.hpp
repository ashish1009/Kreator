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
    
    /// Color of player and Pieces
    /// NOTE: each color is for a player unique so last element is MAX Player
    /// as number of color can never exceed Number of player
    enum Color { Black = 0, White = 1, MAX_PLAYER = 2 };
    
    /// Player information wrapper
    struct Player {
        /// Stores the number of player crerted in the game
        static uint32_t NumPlayerCreated;
    
        std::string Name = "Player"; /// Name of player
        Color Color; /// Color of player
        
        Player(const std::string& name) : Name(name) {
            IK_ASSERT(NumPlayerCreated < MAX_PLAYER, "Num Player Reached max");
            /// Assign the color from enum one by one based on index of player
            Color = (Chess::Color)NumPlayerCreated++;
        }
        
    };
    
    /// Chess Renderer Layer
    class ChessLayer : public Layer {
    public:
        ChessLayer(const std::string& playerName_1, const std::string& playerName_2);
        virtual ~ChessLayer();
        
        void Attach() override;
        void Update(Timestep ts) override;
        void RenderGui() override;
        void Detach() override;
        void EventHandler(Event&) override;
        
        const std::string& GetName() const override { return m_Name; }
        
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        
        void UpdateHoveredEntity();
        
        // Member variabls
        Viewport m_ViewportData;
        
        Player m_Player[MAX_PLAYER];
    };

}
