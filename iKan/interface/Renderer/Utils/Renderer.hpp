//
//  Renderer.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

// Common Renderer File.
// Resposible to create interface between Core and Client
// ALL Renderer API to be called via this class

namespace iKan {

    /// All renderer ID type
    using RendererID = uint32_t;

    /// Interface class to handle ALL Renderer API to be used by client
    class Renderer {
    public:
        /// Supported Renderer API by iKan Engine
        enum class API {
            None = 0,
            OpenGL = 1,
            /* Add other Supported API */};
        
        // Static Methods
        // Core API
        /// Intialize all Renderer that are supported by iKan.
        /// NOTE: to Initialize Client Specific renderer, initialize them in Client Application Constructor
        static void Init();
        
        /// Shutdown all Renderer that are supported by iKan.
        /// NOTE: to Shutdown Client Specific renderer, Shutdown them in Client Application Destructor
        static void Shutdown();
        
        // Renderer_API Controller
        /// Return the Current Set Renderer API Type
        static API GetAPI();
        /// NOTE: Set the Renerer API even before constructing window as Window is creating Renderer Context
        /// Based on the API to be set
        /// @param api Current API need to be set
        static void SetAPI(API api);
        
    private:
        Renderer() = default;
        
        // Member varaibel
        static API s_API;
    };
    
}
