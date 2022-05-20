//
//  Window.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

namespace iKan {
    
    // Forward declaration
    class Event;
    class Timestep;
    
    /// Interface class for creating and managing Window for rendering. Implementation depends on the OS defined.
    /// Responsible for
    /// - Creating Window Instance (For now using GLFW)
    /// - Create Renderer Context and attach to window
    /// - Swap Buffer each frame.
    /// - Update Time step
    /// - Callback Event handler functions
    class Window {
    public:
        /// Specifications of Window
        struct Specification {
            std::string Title = "Kreator";
            uint32_t Width = 2100, Height = 900;

            bool VSync = true;
            bool Fullscreen = false;

            /// if false : Hides the Title bar and close, minimize and Maximize icon also Disable resizable
            bool Decorated = true;

            // Constructors and Destructors
            Specification() = default;
            ~Specification() = default;
            Specification(const Specification&);
        };

        // Using Typedefs
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        // Core Functions
        /// Update Window Each Frame. To be called by application on each frame. It swap the context buffers
        virtual void Update() = 0;
        /// Delete the GLFW Window context
        virtual void Shutdown() = 0;
        /// Update the event callback function.
        /// NOTE: EventCallbackFn to be called every time any even triggered to window
        virtual void SetEventFunction(const EventCallbackFn&) = 0;
        /// Maximise the window. Call when Window need to be maximised
        virtual void Maximize() = 0;
        /// Place window at the center of the screen
        virtual void CenterWindow() = 0;

        // Setters
        /// Change the VSync flag by argiument flag
        virtual void SetVSync(bool vSync) = 0;
        /// Make Window resizable if 'resizable' is true else Window can not be resized after callinh this API
        virtual void SetResizable(bool resizable) const = 0;
        /// Chnage the title of window
        virtual void SetTitle(const std::string& title) = 0;

        // Getters
        virtual bool IsVSync() const = 0;
        virtual bool IsDecorated() const = 0;
        virtual bool IsFullscreen() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual std::string GetTitle() const = 0;
        virtual Timestep GerTimestep() const = 0;
        virtual void* GetNativeWindow() = 0;
        
        /// Static API to Create instance of Window
        /// @param os Operatin system to be used. Use OS enum to check supported OS by iKan
        /// @param spec Window Spcification Data
        static std::unique_ptr<Window> Create(OS os, const Window::Specification& spec);
    };
    
}
