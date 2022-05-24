//
//  Scene.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "Scene.hpp"
#include "Scene/Entity.hpp"

using namespace iKan;

/// Create Secne Reference instance
std::shared_ptr<Scene> Scene::Create() {
    return std::make_shared<Scene>();
}

/// Scene Constructor
Scene::Scene() {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Scene ...");
}

/// Scene Destructor
Scene::~Scene() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Scene!!!");
}

/// Create New Entity to scene
/// @param name Name of entity
/// @param uuid Unique ID for entity
std::shared_ptr<Entity> Scene::CreateEntity(const std::string& name, UUID uuid) {
    auto entity = Entity::Create(m_Registry.create(), this);
//    entity->AddComponent<IDComponent>(uuid);
//    entity->AddComponent<TagComponent>(name);
//    entity->AddComponent<TransformComponent>();
//    
//    IK_CORE_ASSERT((m_EntityIDMap.find(uuid) == m_EntityIDMap.end()), "Entity Already Added");
//    m_EntityIDMap[uuid] = entity;
//    
//    m_MaxEntityId = (uint32_t)(*entity.get());
//    IK_CORE_INFO("Entity '{0}' with ID: {1} is created in the Active Scene", entity->GetComponent<TagComponent>().Tag.c_str(), entity->GetComponent<IDComponent>().ID);
//    IK_CORE_INFO("    Number of entities Added in Scene : {0}, Maximum ID Given to Entity {1}", ++m_NumEntities, m_MaxEntityId);
    
    return entity;
}
