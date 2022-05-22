//
//  OpenGLTexture.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLTexture.hpp"
#include "Renderer/Utils/RendererStats.hpp"
#include <stb_image.h>
#include <glad/glad.h>

using namespace iKan;

#ifdef IK_DEBUG_FEATURE
 
namespace iKan::TextureUtils {
    /// Return the Format name from Enum
    /// @param format enum taken as uint (enum in Glad)
    std::string GetFormatNameFromEnum(uint32_t format) {
        if (format == GL_RGBA8) return "GL_RGBA8";
        if (format == GL_RGBA)  return "GL_RGBA8";
        else IK_CORE_ASSERT(false, "Add New Featured format herer too");
    }
}

#endif
    
/// OpenGL Texture constructor
/// @param width width of texture
/// @param height height of texture
/// @param data texture white data
/// @param size size of texture
OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, void* data, uint32_t size)
: m_Width(width), m_Height(height), m_TextureData(data), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA), m_Size(size) {
    PROFILE();

    while (Renderer::IsTextureRendererIDExist(m_RendererID)) {
        /// Checking is this assihned renderer ID already given to some texture.
        /// If yes then recreate new Texture renderer ID
        // TODO: Decide later to delete older on or not
        glGenTextures(1, &m_RendererID);
    }
    
    Renderer::ManageRendererIDs(m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    uint16_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    IK_CORE_ASSERT((m_Size == m_Width * m_Height * bpp), "Data must be entire texture");
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, (stbi_uc*)(m_TextureData));
    
    RendererStatistics::Get().TextureBufferSize += m_Size;

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Open GL White Texture ... ");
    IK_CORE_INFO("    Renderer ID       : {0}", m_RendererID);
    IK_CORE_INFO("    Width             : {0}", m_Width);
    IK_CORE_INFO("    Height            : {0}", m_Height);
    IK_CORE_INFO("    Size              : {0} Bytes, {1} KB, {2} MB", m_Size, (float)m_Size / 1000.0f, (float)m_Size / 1000000.0f);
    IK_CORE_INFO("    Number of Channel : {0}", m_Channel);
    IK_CORE_INFO("    InternalFormat    : {0}", TextureUtils::GetFormatNameFromEnum(m_InternalFormat));
    IK_CORE_INFO("    DataFormat        : {0}", TextureUtils::GetFormatNameFromEnum(m_DataFormat));
    IK_LOG_SEPARATOR();
}

/// Open GL Texture Destructor
OpenGLTexture::~OpenGLTexture() {
    // TODO: Delete Renderer ID from Texture ID Library stored in Renderer.pp
    PROFILE();

    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Open GL Texture: {0} !!! ", m_Filepath.c_str());
    IK_CORE_WARN("    Renderer ID       : {0}", m_RendererID);
    IK_CORE_WARN("    Width             : {0}", m_Width);
    IK_CORE_WARN("    Height            : {0}", m_Height);
    IK_CORE_WARN("    Size              : {0} Bytes, {1} KB, {2} MB", m_Size, (float)m_Size / 1000.0f, (float)m_Size / 1000000.0f);
    IK_CORE_WARN("    Number of Channel : {0}", m_Channel);
    IK_CORE_WARN("    InternalFormat    : {0}", TextureUtils::GetFormatNameFromEnum(m_InternalFormat));
    IK_CORE_WARN("    DataFormat        : {0}", TextureUtils::GetFormatNameFromEnum(m_DataFormat));
    IK_LOG_SEPARATOR();

    RendererStatistics::Get().TextureBufferSize -= m_Size;
    glDeleteTextures(1, &m_RendererID);
}
