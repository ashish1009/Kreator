//
//  TextureLibrary.cpp
//  iKan
//
//  Created by iKan on 01/06/22.
//

#include "TextureLibrary.hpp"
#include "Renderer/Graphics/Texture.hpp"

using namespace iKan;

std::unordered_map<std::string, std::shared_ptr<Texture>> TextureLibrary::s_TextureLibrary;

/// Generate and store a new Texture if not created already. Else return already created Texture
/// @param path Texture file path
std::shared_ptr<Texture> TextureLibrary::GetTexture(const std::string& path, bool minLinear, bool magLinear) {
    if (s_TextureLibrary.find(path) == s_TextureLibrary.end()) {
        IK_CORE_INFO("Adding Texture '{0}' to Texture Library", StringUtils::GetNameFromFilePath(path));
        s_TextureLibrary[path] = Texture::Create(path, minLinear, magLinear);
    }
    
    return s_TextureLibrary[path];
}

/// Deleting All Texture from library
void TextureLibrary::ResetTextures() {
    for (auto it = s_TextureLibrary.begin(); it != s_TextureLibrary.end(); it++) {
        IK_CORE_WARN("Removing Texture '{0}' from Shdaer Library", StringUtils::GetNameFromFilePath(it->first));
        it->second.reset();
    }
}
