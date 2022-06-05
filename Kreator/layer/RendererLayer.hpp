//
//  RendererLayer.hpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

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
    // Methods
    void UpdateHoveredEntity();
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    
    // variables
    Viewport m_VpData;
    ContentBrowserPannel m_CBP;
    
    std::shared_ptr<Scene> m_ActiveScene;
    std::shared_ptr<SceneHierarchyPannel> m_SHP;
    
    // Temp Data
    std::shared_ptr<SceneCamera> m_SceneCamera;
    std::shared_ptr<EditorCamera> m_EditorCamera;
};

