//
//  MacWindow.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Core/Utils/Timestep.hpp"
#include "Core/Window.hpp"
#include "Renderer/Graphics/RendererContext.hpp"

namespace iKan {
    
    class MacWindow : public Window {
    public:
        /// Mac window Data
        struct Data {
            Window::Specification Specification;
            EventCallbackFn EventCallback;
            
            Data() = default;
            ~Data() = default;
            Data(const Data&);
        };

        // Methods
        MacWindow(const Specification&);
        virtual ~MacWindow();

        // Core Functions
        void Update() override;
        void Shutdown() override;
        void SetEventFunction(const EventCallbackFn& callbackFn) override { m_Data.EventCallback = callbackFn; }
        void Maximize() override;
        void CenterWindow() override;

        // Setters
        void SetVSync(bool flag) override;
        void SetResizable(bool resizable) const override;
        void SetTitle(const std::string& title) override;

        // Getters
        bool IsVSync() const override { return m_Data.Specification.VSync; }
        bool IsDecorated() const override { return m_Data.Specification.Decorated; }
        bool IsFullscreen() const override { return m_Data.Specification.Fullscreen; }
        uint32_t GetWidth() const override { return m_Data.Specification.Width; }
        uint32_t GetHeight() const override { return m_Data.Specification.Height; }
        std::string GetTitle() const override { return m_Data.Specification.Title; }
        Timestep GerTimestep() const override { return m_Time; }
        void* GetNativeWindow() const override { return (void*)m_Window; }

    private:
        // Member Functions
        void Init();
        void SetEventCallbacks();

        // Member veraibles
        GLFWwindow* m_Window;
        Data m_Data;
        Timestep m_Time;
        std::unique_ptr<RendererContext> m_Context;
        TIME m_LastFrameTime = 0.0f;
    };
    
}
