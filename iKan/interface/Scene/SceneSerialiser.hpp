//
//  SceneSerialiser.hpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#pragma once

#include "Scene/Scene.hpp"

namespace iKan {
    
    class SceneSerializer {
    public:
        SceneSerializer(const std::shared_ptr<Scene>& scene);
        ~SceneSerializer();
        
        void Serialize(const std::string& filepath);
        bool Deserialize(const std::string& filepath);
        
    private:
        std::shared_ptr<Scene> m_Scene;
    };
    
}
