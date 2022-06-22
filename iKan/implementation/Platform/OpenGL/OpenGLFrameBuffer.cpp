//
//  OpenGLFrameBuffer.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLFrameBuffer.hpp"
#include <glad/glad.h>

using namespace iKan;

namespace FbUtils {
    
#ifdef IK_DEBUG_FEATURE
    std::string GetTextureFormateStringFromEnum(FrameBuffer::Attachment::TextureFormat format)
    {
        switch (format) {
            case FrameBuffer::Attachment::TextureFormat::RGBA8: return "RGBA8";
            case FrameBuffer::Attachment::TextureFormat::R32I:  return "R32I";
            case FrameBuffer::Attachment::TextureFormat::Depth24Stencil: return "Depth24Stencil";
            case FrameBuffer::Attachment::TextureFormat::DepthCubeMap: return "DepthCubeMap";
            case FrameBuffer::Attachment::TextureFormat::None:
            default:
                IK_CORE_ASSERT(false, "invalid format");
        }
    }
#endif
    
    /// Check is the specification format is depth or not
    /// @param format Format type
    bool IsDepthFormat(FrameBuffer::Attachment::TextureFormat format) {
        switch (format) {
            case FrameBuffer::Attachment::TextureFormat::RGBA8:
            case FrameBuffer::Attachment::TextureFormat::R32I:
                return false;
                
            case FrameBuffer::Attachment::TextureFormat::Depth24Stencil:
            case FrameBuffer::Attachment::TextureFormat::DepthCubeMap:
                return true;
                
            case FrameBuffer::Attachment::TextureFormat::None:
            default:
                IK_CORE_ASSERT(false, "invalid format");
        }
    }
    
    /// Generate texture for framebuffer
    /// @param outID pointer of renderer IDs (number of id to be created)
    /// @param count Number of Tecxture to be created
    static void CreateTextures(uint32_t* outID, uint32_t count) {
        // TODO: Generate all Texuture at once after PBR Shader Testing
        for(int32_t i = 0; i < count; i++) {
            glGenTextures(1, &outID[i]);
            while (Renderer::IsTextureRendererIDExist(outID[i])) {
                /// Checking is this assihned renderer ID already given to some texture.
                /// If yes then recreate new Texture renderer ID
                // TODO: decide later to delete older on or not
                glGenTextures(1, &outID[i]);
            }
            Renderer::AddRendererIDs(outID[i]);
        }
    }
    
    /// Bind the texture attachment created in Framebuffer
    /// @param id Renderer ID to be attached
    static void BindTexture(uint32_t id) {
        glBindTexture(GL_TEXTURE_2D, id);
    }
    
    /// Bind cubemap tecture to FB
    /// @param id Renderer ID to be attached
    static void BindCubeMapTexture(uint32_t id) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }
    
    /// Attach the color attachment to Frame buffer
    /// @param id Renderer ID of tecture format attachment
    /// @param format Texture Format
    /// @param attachmentType attachment type
    /// @param width Width of FB
    /// @param height Height of FB
    /// @param index index for non 0 count of textures
    static void AttachTexture(uint32_t id, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height, int32_t index) {
        /// TODO: Add multisample later when needed
        GLint type = GL_UNSIGNED_BYTE;
        if (format == GL_RGBA8)
            type = GL_UNSIGNED_BYTE;
        if (format == GL_DEPTH_COMPONENT)
            type = GL_FLOAT;
        // handler other formate later
                
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, attachmentType, type, nullptr);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    
    /// Attach ID (Red) Texture to Frame buffer
    /// @param id Renederer ID
    /// @param format Format
    /// @param attachmentType attachment type
    /// @param width Width
    /// @param height Height
    static void AttachIDTexture(uint32_t id, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, attachmentType, GL_UNSIGNED_BYTE, nullptr);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, drawBuffers);
    }
    
    /// Attach Depth Cubemap Texture to Frame buffer
    /// @param id Renderer ID
    /// @param width Width of cubemap
    /// @param height height of cubemap
    static void AttachDepthCubeMapTexture(uint32_t id, uint32_t width, uint32_t height) {
        for (uint32_t i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
        // attach depth texture as FBO's depth buffer
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);
    }
    
    /// Attach Frame buufer texture 2D in framebuffer
    /// @param id renderer ID of attachment
    /// @param attachment attchment of texture
    /// @param type type of tecture
    static void FramebufferTexture(RendererID id, int32_t attachment, int32_t type) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, type, id, 0);
    }
    
}

/// Open GL Framebuffer constructor
/// @param specs Frame buffer specificaiton
OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBuffer::Specification& specs) : m_Specification(specs) {
    Renderer::Submit([this]() {
        IK_LOG_SEPARATOR();
        IK_CORE_INFO("Creating Open GL Framebuffer ...");
        
        for (auto attchmentFormat : m_Specification.Attachments.TextureFormats) {
            if (!FbUtils::IsDepthFormat(attchmentFormat))
                m_ColorSpecifications.emplace_back(attchmentFormat);
            else
                m_DepthSpecification = attchmentFormat;
        }
        Invalidate();
    });
}

/// Open GL Framebuffer destructor
OpenGLFrameBuffer::~OpenGLFrameBuffer() {    
    Renderer::Submit([this]() {
        IK_LOG_SEPARATOR();
        IK_CORE_WARN("Destroying Open GL Framebuffer !!!");
        IK_CORE_WARN("    Renderer ID : {0}", m_RendererID);
        
        uint32_t i = 0;
        IK_CORE_WARN("    ---------------------------------------------");
        IK_CORE_WARN("    Color Attachments ");
        for (const auto& colorSpec : m_ColorSpecifications) {
            Renderer::RemoveRendererIDs(m_ColorAttachments[i]);
            
            IK_CORE_WARN("        Renderer ID : {0}", m_ColorAttachments[i]);
            IK_CORE_WARN("        Formate     : {0}", FbUtils::GetTextureFormateStringFromEnum(colorSpec));
            i++;
        }
        
        Renderer::RemoveRendererIDs(m_DepthAttachment);
        IK_CORE_WARN("    ---------------------------------------------");
        IK_CORE_WARN("    Depth Attachments ");
        IK_CORE_WARN("        Renderer ID : {0}", m_DepthAttachment);
        IK_CORE_WARN("        Formate     : {0}", FbUtils::GetTextureFormateStringFromEnum(m_DepthSpecification));


        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
        
        Renderer::RemoveRendererIDs(m_RendererID);
    });
}

/// Invalidate the Frame buffer after changing any specification
void OpenGLFrameBuffer::Invalidate() {
    IK_CORE_INFO("    Invalidate Open GL Framebuffer");
    
    // Delete the framebuffers if already created
    if (m_RendererID) {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
        
        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }
    
    // Generate the frame buffer to renderer ID
    glGenFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    // Color Attachments
    if (m_ColorSpecifications.size()) {
        m_ColorAttachments.resize(m_ColorSpecifications.size());
        
        IK_CORE_INFO("        -----------------------------------------");
        IK_CORE_INFO("        Creating Color Texture Specifications to FrameBuffer");

        for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
            FbUtils::CreateTextures(&m_ColorAttachments[i], 1);
            FbUtils::BindTexture(m_ColorAttachments[i]);
            
            IK_CORE_INFO("            -------------------------------------");
            IK_CORE_INFO("            Renderer ID : {0}", m_ColorAttachments[i]);
            switch (m_ColorSpecifications[i]){
                case FrameBuffer::Attachment::TextureFormat::None:
                case FrameBuffer::Attachment::TextureFormat::Depth24Stencil:
                case FrameBuffer::Attachment::TextureFormat::DepthCubeMap:
                    break;
                    
                case FrameBuffer::Attachment::TextureFormat::RGBA8:
                    FbUtils::AttachTexture(m_ColorAttachments[i], GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, (uint32_t)i);
                    FbUtils::FramebufferTexture(m_ColorAttachments[i], GL_COLOR_ATTACHMENT0 + (uint32_t)i, GL_TEXTURE_2D);

                    IK_CORE_INFO("            Format      : {0}", ToString(RGBA8));
                    IK_CORE_INFO("            Width       : {0}", m_Specification.Width);
                    IK_CORE_INFO("            Height      : {0}", m_Specification.Width);
                    IK_CORE_INFO("            Index       : {0}", uint32_t(i));

                    break;
                    
                case FrameBuffer::Attachment::TextureFormat::R32I:
                    FbUtils::AttachIDTexture(m_ColorAttachments[i], GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height);
                    FbUtils::FramebufferTexture(m_ColorAttachments[i], GL_COLOR_ATTACHMENT0 + (uint32_t)i, GL_TEXTURE_2D);
                    
                    IK_CORE_INFO("            Format      : {0}", ToString(R32I));
                    IK_CORE_INFO("            Width       : {0}", m_Specification.Width);
                    IK_CORE_INFO("            Height      : {0}", m_Specification.Width);

                    break;
            }
        } // for (size_t i = 0; i < m_ColorAttachments.size(); i++)
    } // Color Attachments
    
    // Depth Attachment
    if (m_DepthSpecification != FrameBuffer::Attachment::TextureFormat::None) {
        
        IK_CORE_INFO("        -----------------------------------------");
        IK_CORE_INFO("        Creating Depth Texture Specifications to FrameBuffer");
        FbUtils::CreateTextures(&m_DepthAttachment, 1);
        IK_CORE_INFO("            -------------------------------------");
        IK_CORE_INFO("            Renderer ID : {0}", m_DepthAttachment);
        switch (m_DepthSpecification) {
            case FrameBuffer::Attachment::TextureFormat::None:
            case FrameBuffer::Attachment::TextureFormat::RGBA8:
            case FrameBuffer::Attachment::TextureFormat::R32I:
                break;
                
            case FrameBuffer::Attachment::TextureFormat::Depth24Stencil:
                FbUtils::BindTexture(m_DepthAttachment);
                FbUtils::AttachTexture(m_DepthAttachment, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, m_Specification.Width, m_Specification.Height, 0);
                FbUtils::FramebufferTexture(m_DepthAttachment, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D);

                IK_CORE_INFO("            Format      : {0}", ToString(Depth24Stencil));
                IK_CORE_INFO("            Width       : {0}", m_Specification.Width);
                IK_CORE_INFO("            Height      : {0}", m_Specification.Width);
                break;
                
            case FrameBuffer::Attachment::TextureFormat::DepthCubeMap:
                FbUtils::BindCubeMapTexture(m_DepthAttachment);
                FbUtils::AttachDepthCubeMapTexture(m_DepthAttachment, 1024, 1024);
                
                IK_CORE_INFO("            Format      : {0}", ToString(Depth24Stencil));
                IK_CORE_INFO("            Width       : {0}", m_Specification.Width);
                IK_CORE_INFO("            Height      : {0}", m_Specification.Width);

                break;
        }
    } // Depth Attachment
    
    if (m_ColorAttachments.size() > 1) {
        IK_CORE_ASSERT((m_ColorAttachments.size() <= 4), "Inalid Attachment");
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
        
        IK_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "FrameBuffer is Incomplete ");
    }
    else if (m_ColorAttachments.empty() && m_DepthSpecification != FrameBuffer::Attachment::TextureFormat::None) {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
        IK_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "FrameBuffer is Incomplete ");
    }
    else {
        IK_CORE_ASSERT(false, "Invalid FrameBuffer Specificaion");
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// Bind the Frame buffer
void OpenGLFrameBuffer::Bind() const {
    Renderer::Submit([this]() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    });
}

/// Bind cubemap texture
/// @param slot Slot of texture bind
void OpenGLFrameBuffer::BindCubemapTexture(uint32_t slot) const {
    Renderer::Submit([this, slot]() {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthAttachment);
    });
}

/// Unbind the frame buffer
void OpenGLFrameBuffer::Unbind() const {
    Renderer::Submit([this]() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    });
}

/// Bind cubemap texture
void OpenGLFrameBuffer::UnbindCubemapTexture() const {
    Renderer::Submit([this]() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    });
}

/// invalidate again on resize
/// @param width width of new Framebuffer
/// @param height height of new Framebuffer
void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
    m_Specification.Width  = width;
    m_Specification.Height = height;
    
    Renderer::Submit([this]() {
        IK_CORE_INFO("Resizing the Framebuffer");
        Invalidate();
    });
}
