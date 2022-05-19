//
//  Application.hpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

namespace iKan {
    
    /// Base class for Core Application. Client can create its own Application derived from Base Core Applicaiton.
    /// Responsible for.
    /// - Initializing all renderers, layers (Include GUI) and Window.
    /// - Render Data each frame (Update Layers and Window).
    /// - Handle Events interrut (Handle All Layer's Events)
    /// - Render GUI window for Interface (Render each Layers GUI Window)
    class Application {
    public:
        /// Application Specification
        struct Specification {
            std::string Name = "Kreator";
            bool StartMaximized = true;
            bool Resizable = true;
            bool EnableGui = true;
            
            // Constructors and Destructors
            Specification() = default;
            ~Specification() = default;
            Specification(const Specification&);
        };
        
        // Constructors
        /// Application Constructor
        /// @param spec Specification defined for application
        Application(const Specification& spec);
        
        // Virtual Methods
        /// Application Destructor
        virtual ~Application();
        
        // Note: Override these Virtual Methods in client Application only if you want to create complete fresh application.
        // and add some specialisation functionality. If these methods will be overriden in client side then functionality
        // will be completely based on overriden method
        /// Update the Application each frame.
        /// Update each Layer for each frame.
        /// Update the window and Swap the buffers
        virtual void Run();
        /// Handle the External event interuption in window
        virtual void EventHandler();
        /// Render GUI Window
        virtual void RenderGui();

        // Static Methods
        /// Return the reference of Application Instance
        static const Application& Get();
        
    private:
        // Copy Constructor
        Application(const Application&) = delete;
        
        // Member variable
        Specification m_Specification;
        
        // Static variables
        static Application* s_Instance;
    };
    
    // NOTE: Client must have the implementation for creating the applicaiton.
    /// Interface for creating instance of Application.
    std::unique_ptr<Application> CreateApplication();
    
}
