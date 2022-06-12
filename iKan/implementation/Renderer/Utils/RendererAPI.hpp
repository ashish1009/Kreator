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
        virtual ~RendererAPI() = default;

        virtual void Init() const = 0;
        virtual void Shutdown() const = 0;
        
        virtual void SetClearColor(const glm::vec4& color) const = 0;
        virtual void ClearBits() const = 0;
        
        virtual void Depth(bool state) const = 0;
        virtual void Blend(bool state) const = 0;
        virtual void MultiSample(bool state) const = 0;
        
        virtual void SetViewPortSize(uint32_t widht, uint32_t height) const = 0;
        
        virtual void GetEntityIdFromPixels(int32_t mx, int32_t my, int32_t& pixelData) const = 0;
        
        virtual void DrawIndexed(const std::shared_ptr<Pipeline>& va, uint32_t count) const = 0;
        virtual void DrawArrays(const std::shared_ptr<Pipeline>& pipeline, uint32_t count) const = 0;
        virtual void DrawIndexedBaseVertex(const std::shared_ptr<Pipeline>& pipeline, uint32_t count, void* indicesdata, uint32_t basevertex) const = 0;

        static std::unique_ptr<RendererAPI> Create();
    };
    
}
