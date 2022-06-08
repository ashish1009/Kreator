//
//  Texture.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "Texture.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

using namespace iKan;

/// Create instance of renderer Texture based on Current Rendere API
/// @param width Width of texture
/// @param height height of Texture
/// @param data white data
/// @param size Size of data
std::shared_ptr<Texture> Texture::Create(uint32_t width, uint32_t height, void* data, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLTexture>(width, height, data, size);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}

/// Create instance of renderer Texture based on Current Rendere API
/// @param path Texture file path
/// @param inverted flag to check if we need to load the image vertically inverted
std::shared_ptr<Texture> Texture::Create(const std::string& path, bool minLinear, bool magLinear) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLTexture>(path, minLinear, magLinear);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}

TextureComponent::TextureComponent(const std::shared_ptr<Texture>& comp, bool use) : Component(comp), Use(use) {}
TextureComponent::TextureComponent(const TextureComponent& other) : Component(other.Component), Use(other.Use) { IK_CORE_INFO("Copying TextureComponent"); }

/// Subtexture Constructir
/// @param texture Texture instnace
/// @param min min bound of sub texture
/// @param max min bound of sub texture
/// @param spriteSize Sprite size (Block size of sub texture)
/// @param cellSize Sprite size (in pixels)
/// @param coords Coordinated of sub texture from Sprite
SubTexture::SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2& spriteSize, const glm::vec2& cellSize, const glm::vec2& coords)
: m_Texture(texture), m_SpriteSize(spriteSize), m_CellSize(cellSize), m_Coords(coords)
{
    m_TexCoord[0] = {min.x, min.y};
    m_TexCoord[1] = {max.x, min.y};
    m_TexCoord[2] = {max.x, max.y};
    m_TexCoord[3] = {min.x, max.y};
}

/// Create Subtexture from a texture sprite
/// @param texture Texture instnace
/// @param coords Coordinated of sub texture from Sprite
/// @param spriteSize Sprite size (Block size of sub texture)
/// @param cellSize Sprite size (in pixels)
std::shared_ptr<SubTexture> SubTexture::CreateFromCoords(const std::shared_ptr<Texture>& texture, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& cellSize)
{
    glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
    glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
    return std::make_shared<SubTexture>(texture, min, max, spriteSize, cellSize, coords);
}

/// Update
void SubTexture::UpdateTexcoords()
{
    glm::vec2 min = { (m_Coords.x * m_CellSize.x) / m_Texture->GetWidth(), (m_Coords.y * m_CellSize.y) / m_Texture->GetHeight() };
    glm::vec2 max = { ((m_Coords.x + m_SpriteSize.x) * m_CellSize.x) / m_Texture->GetWidth(), ((m_Coords.y + m_SpriteSize.y) * m_CellSize.y) / m_Texture->GetHeight() };

    m_TexCoord[0] = {min.x, min.y};
    m_TexCoord[1] = {max.x, min.y};
    m_TexCoord[2] = {max.x, max.y};
    m_TexCoord[3] = {min.x, max.y};
}
