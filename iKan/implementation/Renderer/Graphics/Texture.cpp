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
std::shared_ptr<Texture> Texture::Create(const std::string& path, bool inverted) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLTexture>(path, inverted);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}

TextureComponent::TextureComponent(const std::shared_ptr<Texture>& comp, bool use) : Component(comp), Use(use) {}
TextureComponent::TextureComponent(const TextureComponent& other) : Component(other.Component), Use(other.Use) { IK_CORE_INFO("Copying TextureComponent"); }
