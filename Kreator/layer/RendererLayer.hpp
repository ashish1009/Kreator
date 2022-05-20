//
//  RendererLayer.hpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

class RendererLayer : public Layer {
public:
    RendererLayer();
    virtual ~RendererLayer();
    
    void Attach() override;
    void Detach() override;

    void Update() override;
    void RenderGui() override;
    void EventHandler(Event& event) override;
    
    const std::string& GetName() const override { return m_Name; }
};

