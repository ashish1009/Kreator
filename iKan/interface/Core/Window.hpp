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
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
        virtual void SetEventFunction(const EventCallbackFn&) = 0;
        virtual void Maximize() = 0;
        virtual void CenterWindow() = 0;

        // Setters
        virtual void SetVSync(bool vSync) = 0;
        virtual void SetResizable(bool resizable) const = 0;
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
    };
    
}
