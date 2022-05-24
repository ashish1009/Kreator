//
//  Scene.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "Scene.hpp"
#include "Scene/Entity.hpp"

using namespace iKan;

/// Copy the entity components
/// @param dst Destination entity
/// @param src Source entity
template<typename Component> static void CopyComponentIfExist(Entity& dst, const Entity& src) {
    if (src.HasComponent<Component>())
        dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
}

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
    entity->AddComponent<IDComponent>(uuid);
    entity->AddComponent<TagComponent>(name);
    entity->AddComponent<TransformComponent>();
    
    IK_CORE_ASSERT((m_EntityIDMap.find(uuid) == m_EntityIDMap.end()), "Entity Already Added");
    m_EntityIDMap[uuid] = entity;
    
    m_MaxEntityId = (uint32_t)(*entity.get());
    IK_LOG_SEPARATOR();
    IK_CORE_TRACE("Created Entity in Scene");
    IK_CORE_TRACE("    Name : {0}", entity->GetComponent<TagComponent>().Tag.c_str());
    IK_CORE_TRACE("    ID   : {0}", entity->GetComponent<IDComponent>().ID);
    IK_CORE_TRACE("    Number of entities Added in Scene : {0}", ++m_NumEntities);
    IK_CORE_TRACE("    Maximum ID Given to Entity        : {0}", m_MaxEntityId);
    
    return entity;
}

/// Delete Entoty from Scene
/// @param entity Entity Handle to be removed
void Scene::DestroyEntity(const std::shared_ptr<Entity>& entity) {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Entity from Scene");
    IK_CORE_WARN("    Name : {0}", entity->GetComponent<TagComponent>().Tag.c_str());
    IK_CORE_WARN("    ID   : {0}", entity->GetComponent<IDComponent>().ID);
    IK_CORE_WARN("    Number of entities Added in Scene : {0}", --m_NumEntities);
    
    m_EntityIDMap.erase(entity->GetComponent<IDComponent>().ID);
    m_Registry.destroy(*entity.get());
}

/// Duplicate the entity
/// @param entity Entity Handle to be duplicated
std::shared_ptr<Entity> Scene::DuplicateEntity(const std::shared_ptr<Entity>& entity) {
    PROFILE();
    auto newEntity = CreateEntity(entity->GetName());

    // Copy Components
    CopyComponentIfExist<TagComponent>(*newEntity.get(), *entity.get());
    CopyComponentIfExist<TransformComponent>(*newEntity.get(), *entity.get());

    return newEntity;
}
