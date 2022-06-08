//
//  Texture.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    class Texture;
    /// Texture property used by any component. Also contain flag to render or not
    struct TextureComponent {
        std::shared_ptr<Texture> Component = nullptr;
        bool Use = true;
        
        TextureComponent() = default;
        TextureComponent(const TextureComponent& other);
        TextureComponent(const std::shared_ptr<Texture>& comp, bool use = true);
    };
    
    /// Interface class for Storing Renderer Texture data. Implementation is depending on the
    /// Supported Renerer API.
    class Texture {
    public:
        virtual ~Texture() = default;
        
        /// Return the Width of Texture
        virtual uint32_t GetWidth() const = 0;
        /// Return the Height of Texture
        virtual uint32_t GetHeight() const = 0;
        /// Return the Renderer ID of Texture
        virtual RendererID GetRendererID() const = 0;
        /// Return the File Path of Texture
        /// NOTE: Return "" for white texture
        virtual std::string GetfilePath() const = 0;
        
        /// Bind the Current Texture to a slot of shader
        /// @param slot Slot of shader
        virtual void Bind(uint32_t slot = 0) const = 0;
        /// Unbind the Current Texture from shader slot
        virtual void Unbind() const = 0;
        
        /// Create Emptry Texture with user Defined Data of size height and Width
        /// @param width Width of Empty Texture
        /// @param height Height of Empty Texture
        /// @param data Data to be stored in Empty Texture
        /// @param size Size of type of data stored in Texture
        static std::shared_ptr<Texture> Create(uint32_t width, uint32_t height, void* data, uint32_t size);
        
        /// Create the Texture from image file
        /// @param path path of texture file
        /// @param inverted flag to check if we need to load the image vertically inverted
        static std::shared_ptr<Texture> Create(const std::string& path, bool minLinear = true, bool magLinear = true);
    };
    
    /// Wrepper class to load texture and render as sprite
    class SubTexture {
    public:
        /// Subtexture Constructir
        /// @param texture Texture instnace
        /// @param min min bound of sub texture
        /// @param max min bound of sub texture
        /// @param spriteSize Sprite size (Block size of sub texture)
        /// @param cellSize Sprite size (in pixels)
        /// @param coords Coordinated of sub texture from Sprite
        SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2& spriteSize = {1.0f, 1.0f}, const glm::vec2& cellSize = {16.0f, 16.0f}, const glm::vec2& coords = {0.0f, 0.0f});
        ~SubTexture() = default;
        
        /// return the texture reference of sprite
        const std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
        /// return Texture coordinates of Subtexture
        const glm::vec2* GetTexCoord() const { return m_TexCoord; }
        /// return Sprite size of Subtexture
        glm::vec2& GetSpriteSize() { return m_SpriteSize; }
        /// return Cell Size of Subtexture
        glm::vec2& GetCellSize() { return m_CellSize; }
        /// return Coordinates of Subtexture
        glm::vec2& GetCoords() { return m_Coords; }

        /// Create Subtexture from a texture sprite
        /// @param texture Texture instnace
        /// @param coords Coordinated of sub texture from Sprite
        /// @param spriteSize Sprite size (Block size of sub texture)
        /// @param cellSize Sprite size (in pixels)
        static std::shared_ptr<SubTexture> CreateFromCoords(const std::shared_ptr<Texture>& texture, const glm::vec2& coords, const glm::vec2& spriteSize = {1.0f, 1.0f}, const glm::vec2& cellSize = {16.0f, 16.0f});

    private:
        
        void UpdateTexcoords();

        std::shared_ptr<Texture> m_Texture;
        glm::vec2 m_TexCoord[4];
        glm::vec2 m_SpriteSize;
        glm::vec2 m_CellSize;
        glm::vec2 m_Coords;
    };
}
