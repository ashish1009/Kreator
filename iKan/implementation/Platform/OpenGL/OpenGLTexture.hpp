//
//  OpenGLTexture.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Graphics/Texture.hpp"

namespace iKan {
    
#ifdef IK_DEBUG_FEATURE
    
namespace TextureUtils {
    /// Return the Format name from Enum
    /// @param format enum taken as uint (enum in Glad)
    std::string GetFormatNameFromEnum(uint32_t format);
}
    
#endif
    
    class OpenGLTexture : public Texture {
    public:
        // Constructors and Destructors
        OpenGLTexture(const std::string& path, bool minLinear, bool magLinear);
        OpenGLTexture(uint32_t width, uint32_t height, void* data, uint32_t size);
        virtual ~OpenGLTexture();

        // Getters
        uint32_t GetWidth() const override { return m_Width;  }
        uint32_t GetHeight() const override { return m_Height; }
        RendererID GetRendererID() const override { return m_RendererID; }
        std::string GetfilePath() const override { return m_Filepath; }
        
        // Fundamentals
        void Bind(uint32_t slot = 0) const override;
        void Unbind() const override;

    private:
        // Member variables
        bool m_Uploaded = false;
        
        RendererID m_RendererID = 0;
        
        int32_t m_Width = 0, m_Height = 0;
        int32_t m_Channel = 0;
        uint32_t m_Size = 0;
        uint32_t m_InternalFormat = 0, m_DataFormat = 0;
        
        void*  m_TextureData;
        std::string m_Filepath = "";
    };
    
    /// Implementaiton for Open GL Char Texture
    class OpenGLCharTexture : public CharTexture {
    public:
        OpenGLCharTexture(const FT_Face& face, const glm::ivec2& size, const glm::ivec2& bearing, uint32_t advance);
        virtual ~OpenGLCharTexture() = default;
 
        RendererID GetRendererID() const override;
        void Bind(uint32_t slot = 0) const override;
        void Unbind() const  override;
        
        glm::ivec2 GetSize() const override { return m_Size; }
        glm::ivec2 GetBearing() const override { return m_Bearing; }
        uint32_t GetAdvance() const override { return m_Advance; }

    private:
        RendererID m_RendererID;
        glm::ivec2 m_Size;
        glm::ivec2 m_Bearing;
        uint32_t   m_Advance;
    };
    
}
