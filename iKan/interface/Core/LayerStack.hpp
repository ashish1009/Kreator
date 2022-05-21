//
//  Layerstack.hpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include "Core/Layer.hpp"

namespace iKan {

    /// Stack to store all the Layers used by application
    /// Responsible for Insertion, deletion and Iteration of layers
    class LayerStack {
    public:
        // Constructors and Destructors
        LayerStack();
        ~LayerStack();

        /// Push the layer in Stack
        /// @param layer layer shaerd pointer to be inserted
        void PushLayer(const std::shared_ptr<Layer>& layer);

        /// Erase the layer from the Stack
        /// @param layer layer shared pointer to be deleted
        void PopLayer(const std::shared_ptr<Layer>& layer);

        // --------------- APIs for Iterators -------------------------
        std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
        std::vector<std::shared_ptr<Layer>>::iterator end() { return m_Layers.end(); }
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

        std::vector<std::shared_ptr<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
        std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return m_Layers.end(); }
        std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
        std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

    private:
        uint32_t m_NumLayers = 0;
        std::vector<std::shared_ptr<Layer>> m_Layers;
    };
    
}
