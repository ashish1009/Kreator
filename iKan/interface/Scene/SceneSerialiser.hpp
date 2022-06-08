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
        /// Create instanc of Scene serialize and store the scene
        /// @param scene scene pointer
        SceneSerializer(const std::shared_ptr<Scene>& scene);
        /// Scene Serializer Destructor
        ~SceneSerializer();
        
        /// Serialize / Save the scene at path
        /// @param filepath path wehre scene need to be saved
        void Serialize(const std::string& filepath);
        /// Deserialize / Load the Scene from path
        /// @param filepath path which need to be loaded
        bool Deserialize(const std::string& filepath);
        
    private:
        std::shared_ptr<Scene> m_Scene;
    };
    
}
