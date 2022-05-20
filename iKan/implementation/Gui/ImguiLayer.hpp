//
//  ImguiLayer.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Core/Layer.hpp"

namespace iKan {
    
    /// Manager Imgui Layer in application
    class ImguiLayer : public Layer {
    public:
        ImguiLayer();
        
        virtual ~ImguiLayer();
        
        void Attach() override;
        void Detach() override;
        void EventHandler(Event& event) override;
        
        void Update(Timestep ts) override;
        void RenderGui() override;
        
        const std::string& GetName() const override;
        
        void Begin();
        void End();
        
        void BlockEvents(bool);
        
    private:
        bool m_BlockEvents = true;
    };
    
}
