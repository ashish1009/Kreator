//
//  ShaderLibrary.hpp
//  iKan
//
//  Created by iKan on 22/05/22.
//

#pragma once

namespace iKan {
    
    class Shader;
    class Renderer;
    class ShaderLibrary {
    private:
        ShaderLibrary() = default;

        static std::shared_ptr<Shader> GetShader(const std::string& path);
        static void ResetShaders();

        static std::unordered_map<std::string, std::shared_ptr<Shader>> s_ShaderLibrary;
        
        friend class Renderer;
    };
    
}
