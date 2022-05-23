//
//  OpenGLFrameBuffer.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"

namespace iKan {
    
    /// Implementation to store Open GL Framebuffer
    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(const Specification& spec);
        virtual ~OpenGLFrameBuffer();

        void Bind() const override;
        void Unbind() const override;

        void BindCubemapTexture(uint32_t slot = 0) const override;
        void UnbindCubemapTexture() const override;

        void Resize(uint32_t width, uint32_t height) override;
        
        const Specification& GetSpecification() const override { return m_Specification; }
        
        RendererID GetRendererId() const override { return m_RendererID; }
        RendererID GetDepthAttachmentId() const override { return m_DepthAttachment; }
        const std::vector<RendererID>& GetColorAttachmentIds() const override { return m_ColorAttachments; }

    private:
        // Member Functions
        void Invalidate();
        
        // Member variables
        RendererID m_RendererID = 0;
        Specification m_Specification;
        
        // Stores the attachments of Framebuffer
        std::vector<Attachment::TextureFormat> m_ColorSpecifications;
        Attachment::TextureFormat m_DepthSpecification = Attachment::TextureFormat::None;
        
        // Stores the attachment ID for both color and depth
        std::vector<RendererID> m_ColorAttachments;
        RendererID m_DepthAttachment = 0;
    };
    
}
