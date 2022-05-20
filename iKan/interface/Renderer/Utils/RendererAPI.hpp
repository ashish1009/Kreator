//
//  RendererAPI.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

namespace iKan {
    
    /// Interface for Renderer API
    /// NOTE: If Not using iKan Renderer then create instance of Renderee API in Client side
    /// By default Application is taking care of all
    class RendererAPI {
    public:
        /// Stores the capability of renderer
        struct Capabilities {
            std::string Vendor;
            std::string Renderer;
            std::string Version;
            
            static Capabilities& Get() {
                static Capabilities capabilities;
                return capabilities;
            }
    
        private:
            ~Capabilities() = default;
            Capabilities() = default;
            Capabilities(const Capabilities&) = delete;
            Capabilities(Capabilities&&) = delete;
            Capabilities& operator=(const Capabilities&) = delete;
            Capabilities& operator=(Capabilities&&) = delete;
        };

        virtual ~RendererAPI() = default;

        virtual void Init() const = 0;
        virtual void Shutdown() const = 0;
        
        virtual void SetClearColor(const glm::vec4& color) const = 0;
        virtual void ClearBits() const = 0;
        
        virtual void Depth(bool state) const = 0;
        virtual void Blend(bool state) const = 0;
        virtual void MultiSample(bool state) const = 0;
        
        static std::unique_ptr<RendererAPI> Create();
    };
    
}
