//
//  RendererLayer.hpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include <iKanHeaders.h>
#include "mario/Player.hpp"

using namespace iKan;

namespace Mario {
    
    /// Data used for Mario Game. Both Rendering and Computation
    struct Data {
        bool IsStarted = false;         // Flag to check is game started. Set up only when se select number of player
        uint32_t NumPlayers = 0;        // Stores the number of player
        std::vector<Player> Players;    // Stores information of each players in vector (Make sure to iterate till NumPlayers only)
    };
    
    /// Chess Renderer Layer
    class MarioLayer : public Layer {
    public:
        MarioLayer();
        virtual ~MarioLayer();
        
        void Attach() override;
        void Update(Timestep ts) override;
        void RenderGui() override;
        void Detach() override;
        void EventHandler(Event&) override;
        
        const std::string& GetName() const override { return m_Name; }
        
    private:
        // Layer API
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        void UpdateHoveredEntity();
                
        // Member variabls
        Viewport m_ViewportData;
        std::shared_ptr<Scene> m_Scene;
        std::shared_ptr<Entity> m_CameraEntity;
        
        // Mario Data
        Data m_Data;
        
        // Mario Data before game start
        float m_SelectPlayerIconPosition = -4.8f;
    };

}
