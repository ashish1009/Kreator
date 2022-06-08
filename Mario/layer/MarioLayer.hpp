//
//  RendererLayer.hpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Mario {
    
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
        
        // Member variabls
        // Layer Data
        Viewport m_ViewportData;
        std::shared_ptr<Scene> m_Scene;

        std::shared_ptr<Entity> m_CameraEntity;
    };

}
