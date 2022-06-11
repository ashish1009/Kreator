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
    void OnImguizmoUpdate();
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    bool OnKeyPressed(KeyPressedEvent& event);
    
    // Scene Handler
    const std::shared_ptr<Scene>& NewScene();
    const std::shared_ptr<Scene>& OpenScene(const std::string& path);
    const std::shared_ptr<Scene>& SaveScene();
    void CloseScene();
    
    // variables
    Viewport m_VpData;
    ContentBrowserPannel m_CBP;
    
    std::shared_ptr<Scene> m_ActiveScene;
    std::shared_ptr<SceneHierarchyPannel> m_SHP;
    
    // Temp Data
    std::shared_ptr<SceneCamera> m_SceneCamera;
    std::shared_ptr<EditorCamera> m_EditorCamera;
    
    std::shared_ptr<Shader> m_TextShader;
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
};

