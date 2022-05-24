//
//  Scene.hpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#pragma once

#include <entt.hpp>
#include "Core/Event/Event.h"
#include "Core/Math/UUID.hpp"
#include "Editor/EditorCamera.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Scene/SceneCamera.hpp"

namespace iKan {

    // Forward Declarations
    class Entity;
    class SceneHierarchyPannel;
    class SceneSerializer;
    
    /// Stores all infor and data of scene
    class Scene {
    public:
        /// Scene Constructor
        Scene();
        /// Scene Destructor
        ~Scene();
        
        /// Create Entity and store in Scene registry
        /// @param name of entity
        /// @param uuid Unique ID of Entity
        std::shared_ptr<Entity> CreateEntity(const std::string& name = "Unknown Entity", UUID uuid = UUID());
        /// Duplicate the entity and add new to Scene
        /// @param entity entity pointer reference
        std::shared_ptr<Entity> DuplicateEntity(const std::shared_ptr<Entity>& entity);
        /// Delete the entity from Scene
        /// @param entity Entity pointer reference
        void DestroyEntity(const std::shared_ptr<Entity>& entity);
        
        /// Create Scene instance
        static std::shared_ptr<Scene> Create();
        
    private:
        // Registry to store all entity
        entt::registry m_Registry;

        // Store the entity ID Map
        std::unordered_map<UUID, std::shared_ptr<Entity>> m_EntityIDMap;

        // Number of Entity stored in Scene and Max ID given to Entity
        uint32_t m_NumEntities = 0, m_MaxEntityId = 0;
        
        friend class Entity;
        friend class SceneHierarchyPannel;
        friend class SceneSerializer;
    };
    
}
