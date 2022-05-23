//
//  FrameBuffer.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    /// Interface to store the Graphics framebuffer
    class FrameBuffer {
    public:
        /// Stores the Attachments of Frame buffer.
        /// NOTE: Separate class to use initializer list in constructor of Specification
        struct Attachment {
            /// Texture format type
            enum class TextureFormat { None = 0, RGBA8, R32I, Depth24Stencil, DepthCubeMap};
            
            Attachment() = default;
            Attachment(std::initializer_list<TextureFormat> attachments) : TextureFormats(attachments) { }
            Attachment(const Attachment& other);
            Attachment& operator=(const Attachment&);

            std::vector<TextureFormat> TextureFormats;
        };
        
        /// Framebuffer specification
        struct Specification {
            uint32_t Width = 2100, Height = 900;
            glm::vec4 Color = { 0.1f, 0.1f, 0.1f, 1.0f };
            Attachment Attachments;
            
            Specification() = default;
            Specification(const Specification&);
            Specification& operator=(const Specification&);
        };

        ~FrameBuffer() = default;
        
        /// Bind the current FrameBuffer to the renderer
        virtual void Bind() const = 0;
        /// Unbind the current FrameBuffer from the renderer
        virtual void Unbind() const = 0;
        
        /// Bind the Cubemap Texture to current FrameBuffer at slot
        /// @param slot Slot of Shader where Cubemapt texture to be binded
        virtual void BindCubemapTexture(uint32_t slot = 0) const = 0;
        /// Unbind the Cubemap Texture from Shader
        virtual void UnbindCubemapTexture() const = 0;

        /// Resize the Framebuffer
        /// @param width New width of FB
        /// @param height New height of FB
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        
        /// Return the Frame buffer specification
        virtual const Specification& GetSpecification() const = 0;

        /// Return the Renderer ID
        virtual RendererID GetRendererId() const = 0;
        /// Return the Depth Attachment ID
        virtual RendererID GetDepthAttachmentId() const = 0;
        /// Return the Color Attachment iDs
        virtual const std::vector<RendererID>& GetColorAttachmentIds() const = 0;

        /// Static API to create the Framebuffer instacne based on the current Supported API
        /// @param spec Frame buffer specification
        static std::shared_ptr<FrameBuffer> Create(const Specification& spec);
    };

    
}
