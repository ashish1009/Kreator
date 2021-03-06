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
    /// NOTE: If Not using iKan Renderer then create instance of Imgui Layer in Client Application and manage API Accordingly
    /// By default Application is taking care of all
    class ImguiLayer : public Layer {
    public:
        ImguiLayer();
        
        virtual ~ImguiLayer();
        
        // ----------------- Overriden methods --------------------
        // For details of Calls, visit Layer.hpp
        void Attach() override;
        void Detach() override;
        void EventHandler(Event& event) override;
        
        void Update(Timestep ts) override;
        void RenderGui() override;
        
        const std::string& GetName() const override;
        
        /// Call begin function before rendering Imgui
        /// NOTE: Call once per frame
        void Begin();
        /// Call the end function after rendering All Imgui Windows
        /// NOTE: Call once per frame
        void End();
        
        /// Block the Imgui Events if flag is true
        void BlockEvents(bool);
        
        /// Override font
        /// @param defaultFont Default font path
        /// @param boldFont bold Font Path
        void SetFont(const std::string &defaultFont, const std::string &boldFont);
        
    private:
        bool m_BlockEvents = true;
    };
    
}
