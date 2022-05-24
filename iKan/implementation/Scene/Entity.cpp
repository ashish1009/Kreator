//
//  Entity.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "Entity.hpp"

using namespace iKan;

std::shared_ptr<Entity> Entity::Create(entt::entity handle, Scene* scene) {
    return std::make_shared<Entity>(handle, scene);
}

/// Entity Constructor
/// @param handle Handle of entity
/// @param scene Scene pointer
Entity::Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {
    IK_CORE_INFO("Creating Entity with Handle: {0} ...", (uint32_t)m_EntityHandle);
}

Entity::~Entity() {
    IK_CORE_WARN("Destroying Entity with Handle: {0} !!!", (uint32_t)m_EntityHandle);
}

UUID Entity::GetUUID() { return GetComponent<IDComponent>().ID; }
const std::string& Entity::GetName() { return GetComponent<TagComponent>().Tag; }

Entity::operator entt::entity() const { return m_EntityHandle; }
Entity::operator bool() const { return m_EntityHandle != entt::null; }
Entity::operator uint32_t() const { return (uint32_t)m_EntityHandle; }

bool Entity::operator ==(const Entity& other) { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
bool Entity::operator !=(const Entity& other) { return !(*this == other); }

bool Entity::operator ==(const entt::entity& other) { return m_EntityHandle == other; }
bool Entity::operator !=(const entt::entity& other) { return !(*this == other); }
