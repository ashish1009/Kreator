//
//  Entity.hpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#pragma once

#include "Scene/Scene.hpp"
#include "Scene/Component.hpp"

namespace iKan {
    
    /// enTT Wrapper class to get, add .. APIs
    class Entity {
    public:
        /// Entity Constructor
        /// @param handle handle of Entity. Scene (registry) takes cares for this
        /// @param scene Scene pointer
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = delete;
        Entity(Entity&& other) = delete;
        /// Destrcutor
        ~Entity();
        
        /// Add component in Current Entity
        /// @param args Arguments needed to construct the component
        /// NOTE : T is type of component
        template<typename T, typename... Args> T& AddComponent(Args&&... args) {
            IK_ASSERT(!HasComponent<T>(), "Entity already has component!");
            T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            return component;
        }
        
        /// Add component in Current Entity (replace if already present)
        /// @param args Arguments needed to construct the component
        /// NOTE : T is type of component
        template<typename T, typename... Args> T& AddOrReplaceComponent(Args&&... args) {
            T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
            return component;
        }
        
        /// return component from Current Entity
        /// NOTE : T is type of component
        template<typename T> T& GetComponent() const {
            IK_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }
        
        /// check if Entity have a component
        /// NOTE : T is type of component
        template<typename T> bool HasComponent() const {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }
        
        /// Remove component from Current Entity
        /// NOTE : T is type of component
        template<typename T> void RemoveComponent() {
            IK_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }
        
        /// return the UUID of Current entity
        UUID GetUUID();
        /// return name of entity
        const std::string& GetName();
        
        /// return Entity as entity handle
        operator entt::entity() const;
        /// Return entity as Boolean
        /// Check if entity handle is not null
        operator bool() const;
        
        /// return the entity handle from this Entity
        operator uint32_t() const;
        
        /// check if this entity is equal to other
        /// @param other other entity
        bool operator ==(const Entity& other);
        /// check if this entity is not equal to other
        /// @param other other entity
        bool operator !=(const Entity& other);
        
        /// check if this entity is equal to other (only handle)
        /// @param other other entity handle
        bool operator ==(const entt::entity& other);
        /// check if this entity is not equal to other (only handle)
        /// @param other other entity handle
        bool operator !=(const entt::entity& other);
        
        /// Create Shared pointer of Entity
        /// @param handle Entity Handler
        /// @param scene Scene pointer
        static std::shared_ptr<Entity> Create(entt::entity handle, Scene* scene);
        
    private:
        entt::entity m_EntityHandle{ entt::null };
        Scene* m_Scene;
    };
    
}
