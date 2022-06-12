//
//  Layerstack.cpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#include "LayerStack.hpp"

using namespace iKan;

/// LayerStack Constructor
LayerStack::LayerStack() {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Layerstack ...");
}

/// LayerStack Destructr
LayerStack::~LayerStack() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Layerstack !!!");
    IK_CORE_WARN("    Deleting all Layers from Layerstack");
    for (auto& layer : m_Layers) {
        IK_CORE_WARN("Poping the layer : '{0}', from the stack. (Total Layers : {1})", layer->GetName().c_str(), m_NumLayers--);
        layer->Detach();
    }
}

/// Push the New layer in the stack
/// @param layer layer pointer to be inserted in the stack
void LayerStack::PushLayer(const std::shared_ptr<Layer>& layer) {
    if (std::find(m_Layers.begin(), m_Layers.end(), layer) != m_Layers.end())
        return;

    IK_CORE_INFO("Pushing the Layer : '{0}' in stack. (Total Number of Layers : {1})", layer->GetName().c_str(), ++m_NumLayers);
    m_Layers.emplace_back(layer);
    layer->Attach();
}

/// Pop the New layer in the stack
/// @param layer layer pointer to be deleted from the stack
void LayerStack::PopLayer(const std::shared_ptr<Layer>& layer) {
    IK_CORE_WARN("Poping the layer : '{0}', from the stack. (Total Layers  Left : {1})", layer->GetName().c_str(), --m_NumLayers);
    if (auto it = std::find(m_Layers.begin(), m_Layers.end(), layer); it != m_Layers.end()) {
        layer->Detach();
        m_Layers.erase(it);
    }
}
