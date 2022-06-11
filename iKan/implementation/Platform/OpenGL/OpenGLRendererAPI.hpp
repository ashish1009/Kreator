//
//  OpenGLRendererAPI.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include "Renderer/Utils/RendererAPI.hpp"

namespace iKan {

    class OpenGLRendererAPI : public RendererAPI {
    public:
        OpenGLRendererAPI();
        virtual ~OpenGLRendererAPI();
        
        void Init() const override;
        void Shutdown() const override;
        
        void SetClearColor(const glm::vec4& color) const override;
        void ClearBits() const override;
        
        void Depth(bool state) const override;
        void Blend(bool state) const override;
        void MultiSample(bool state) const override;

        void SetViewPortSize(uint32_t widht, uint32_t height) const override;
        
        void GetEntityIdFromPixels(int32_t mx, int32_t my, int32_t& pixelData) const override;
        
        void DrawIndexed(const std::shared_ptr<Pipeline>& pipeline, uint32_t count) const override;
        void DrawArrays(const std::shared_ptr<Pipeline>& pipeline, uint32_t count) const override;
        void DrawIndexedBaseVertex(const std::shared_ptr<Pipeline>& pipeline, uint32_t count, void* indicesdata, uint32_t basevertex) const override;
    };
    
}
