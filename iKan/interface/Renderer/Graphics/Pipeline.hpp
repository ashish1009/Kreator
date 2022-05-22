//
//  Pipeline.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    /// Interface class for Storing Renderer Pipeline. Implementation is depending on the
    /// Supported Renerer API.
    class Pipeline {
    public:
        virtual ~Pipeline() = default;
        
        /// Bind the Pipeline before rendering
        virtual void Bind() const = 0;
        /// Unbind the Pipeline after rendering
        virtual void Unbind() const = 0;

        /// Return the renderer ID of Pipeline
        virtual RendererID GetRendererID() const = 0;

        /// Static API to Create instance of Pipeline
        static std::shared_ptr<Pipeline> Create();
    };
    
}
