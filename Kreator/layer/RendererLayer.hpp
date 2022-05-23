//
//  RendererLayer.hpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

/// Viewport Data
struct Viewport {
    std::shared_ptr<FrameBuffer> FrameBuffer;
};

/// Kreator's Renderer Layer
class RendererLayer : public Layer {
public:
    RendererLayer();
    virtual ~RendererLayer();
    
    void Attach() override;
    void Detach() override;

    void Update(Timestep ts) override;
    void RenderGui() override;
    void EventHandler(Event& event) override;
    
    const std::string& GetName() const override { return m_Name; }
    
private:
    Viewport m_VpData;
    
    // Temp Data
    std::shared_ptr<SceneCamera> m_SceneCamera;
};

