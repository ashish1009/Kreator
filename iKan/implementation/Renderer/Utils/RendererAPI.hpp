//
//  RendererAPI.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

namespace iKan {
    
    // Forward Declaration
    class Pipeline;
    
    /// Interface for Renderer API
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
        
        virtual void SetViewPortSize(uint32_t widht, uint32_t height) const = 0;
        
        virtual void DrawIndexed(const std::shared_ptr<Pipeline>& va, uint32_t count) const = 0;
        virtual void DrawIndexedBaseVertex(const std::shared_ptr<Pipeline>& pipeline, uint32_t count, void* indicesdata, uint32_t basevertex) const = 0;

        static std::unique_ptr<RendererAPI> Create();
    };
    
}
