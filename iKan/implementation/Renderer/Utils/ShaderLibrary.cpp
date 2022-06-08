//
//  ShaderLibrary.cpp
//  iKan
//
//  Created by iKan on 22/05/22.
//

#include "ShaderLibrary.hpp"
#include "Renderer/Graphics/Shader.hpp"

using namespace iKan;

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::s_ShaderLibrary;

/// Generate and store a new shader if not created already. Else return already created Shader
/// @param path Shader file path
std::shared_ptr<Shader> ShaderLibrary::GetShader(const std::string& path) {
    if (s_ShaderLibrary.find(path) == s_ShaderLibrary.end()) {
        IK_CORE_INFO("Adding Shader '{0}' to Shdaer Library", StringUtils::GetNameFromFilePath(path));
        s_ShaderLibrary[path] = Shader::Create(path);
    }
    else {
        IK_CORE_INFO("Returning Pre loaded Shader '{0}' from Shdaer Library", StringUtils::GetNameFromFilePath(path));
    }
    
    return s_ShaderLibrary[path];
}

/// Deleting All shader from library
void ShaderLibrary::ResetShaders() {
    for (auto it = s_ShaderLibrary.begin(); it != s_ShaderLibrary.end(); it++) {
        IK_CORE_WARN("Removing Shader '{0}' from Shdaer Library", StringUtils::GetNameFromFilePath(it->first));
        it->second.reset();
    }
}
