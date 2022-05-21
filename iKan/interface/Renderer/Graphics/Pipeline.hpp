//
//  Pipeline.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

namespace iKan {
    
    /// Interface class for Storing Renderer Pipeline. Implementation is depending on the
    /// Supported Renerer API.
    class Pipeline {
    public:
        /// Static API to Create instance of Pipeline
        static std::shared_ptr<Pipeline> Create();
    };
    
}
