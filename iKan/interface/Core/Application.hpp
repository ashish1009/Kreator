//
//  Application.hpp
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include "Core/Window.hpp"
#include "Core/Utils/Timestep.hpp"
#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    // Forward Declarations
    class WindowCloseEvent;
    class LayerStack;
    class Layer;
    class ImguiLayer;

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
            OS Os = OS::None;
            Renderer::API RendererAPI = Renderer::API::None;;
            bool StartMaximized = true;
            bool Resizable = true;
            bool EnableGui = true;
            
            // If window specification not set manully, iKan will pick default values
            Window::Specification WindowSpec;

            // Constructors and Destructors
            Specification() = default;
            ~Specification() = default;
            Specification(const Specification&);
            Specification& operator=(const Specification&);
        };

        // --------------- Constructors -------------------------
        /// Application Constructor
        /// @param spec Specification defined for application
        Application(const Specification& spec);

        // ------------ Virtual Methods -------------------------
        /// Application Destructor
        virtual ~Application();

        // Note: Override these Virtual Methods in client Application only if you want to create complete fresh application
        // and add some specialisation functionality. If these methods will be overriden in client side then functionality
        // will be completely based on overriden method
        /// Update the Application each frame.
        /// Update each Layer for each frame.
        /// Update the window and Swap the buffers
        virtual void Run();
        /// Handle the External event interuption in window
        /// @param event Event (Base class) intance. Dispatch event from Event Dispatcher
        virtual void EventHandler(Event& event);
        /// Render GUI Window
        virtual void RenderGui();
        /// Close the Current Application Game Loop
        virtual void Close();

        // -------------- Non Virtual Methods -------------------
        /// Push the layer in Core Application Layer stack
        /// @param layer Layer pointer to be added
        void PushLayer(const std::shared_ptr<Layer>& layer);
        
        /// Pop the layer in Core Application Layer stack
        /// @param layer Layer pointer to be errased
        void PopLayer(const std::shared_ptr<Layer>& layer);
        
        // -------------------- Getters -------------------------
        /// Return the GLFW Window native
        void* GetWindowPtr() const;
        /// Return the iKan Window Instance reference
        const Window& GetWindow() const;
        /// Return the Imgui Layer pointer Reference
        ImguiLayer& GetImGuiLayer() const;

        // --------------- Static Methods -----------------------
        /// Return the reference of Application Instance
        static const Application& Get();

    private:
        // Deleting Copy Constructor
        Application(const Application&) = delete;

        // Member Methods
        void Init();
        bool WindowClose(WindowCloseEvent& windowEvent);

        // Member variable
        bool m_IsRunning = true;
        Specification m_Specification;
        Timestep m_Timestep;
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<LayerStack> m_LayerStack;
        std::shared_ptr<ImguiLayer> m_ImguiLayer;

        // Static variables
        static Application* s_Instance;
    };

    // NOTE: Client must have the implementation for creating the applicaiton.
    /// Interface for creating instance of Application.
    std::unique_ptr<Application> CreateApplication();

}
