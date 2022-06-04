//
//  SceneHierarchyPannel.cpp
//  iKan
//
//  Created by iKan on 03/06/22.
//

#include "SceneHierarchyPannel.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Editor/PropertyGrid.hpp"

using namespace iKan;

/// Create instance of Scene Hierarchy pannel
/// @param context context reference pointer
std::shared_ptr<SceneHierarchyPannel> SceneHierarchyPannel::Create(const std::shared_ptr<Scene>& context) {
    return std::make_shared<SceneHierarchyPannel>(context);
}

/// Scene Hierarchy pannel constructor
/// @param scene active Scene instance
SceneHierarchyPannel::SceneHierarchyPannel(const std::shared_ptr<Scene>& context) : m_Context(context) {
    IK_CORE_ASSERT(m_Context, "Context is null");
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Scene Hierarchy pannel ...");
}

/// Scene Hierarchy pannel Destructir
SceneHierarchyPannel::~SceneHierarchyPannel() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Scene Hierarchy pannel !!!");
}

/// Render Scene Hierarchy pannel imgui to show scene Info
void SceneHierarchyPannel::RenderImgui() {
    // Scene Info
    ImGui::Begin("Scene Data");
    ImGui::PushID("Scene Data");
    
    // Show Scene Na
    ImGui::Text(" %s (%d Entities. Max Entity ID: %d)", m_Context->GetName().c_str(), m_Context->GetNumEntities(), m_Context->GetMaxEntityId());
    ImGui::Separator();
    
    // Search filter
    static ImGuiTextFilter entityFilter;
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 16);
    if (PropertyGrid::Search(entityFilter.InputBuf, "Search ... "))
        entityFilter.Build();
    ImGui::Separator();
    
    // Show Group of entities
    for (auto& [group, entities] : m_EntityGroups) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
        bool opened = ImGui::TreeNodeEx((void*)(group.c_str()), flags, group.c_str());
        
        if (opened) {
            for (const auto& entity : entities) {
                auto& tag = entity->GetComponent<TagComponent>().Tag;
                
                if(entityFilter.PassFilter(tag.c_str()))
                    DrawEntityTreeNode(entity);
            }
            ImGui::TreePop();
        }
    }
    
    // Group each entity
    m_Context->m_Registry.each([&](auto entityID)
                               {
        // We are creating a local entity but it is containing the same entity handle or entt::entity, in the same scene (active)
        GroupEntities(m_Context->m_EntityIDMap[m_Context->m_Registry.get<IDComponent>(entityID).ID]);
    });
    
    // Reset the selected entity
    if (ImGui::IsMouseDown((int32_t)MouseButton::ButtonLeft) && ImGui::IsWindowHovered())
        m_SelectedEntity = nullptr;
    
    // Menu popup on Right Click
    if (ImGui::BeginPopupContextWindow(0, (int32_t)MouseButton::ButtonRight, false)) { // false -> Right-click on blank space
        if (ImGui::BeginMenu("Create Empty Entity")) {
            if (ImGui::MenuItem("Empty Entity"))
                m_Context->CreateEntity("Empty Entity");
            ImGui::Separator();
            
            if (ImGui::MenuItem("Camera")) {
                auto entity = m_Context->CreateEntity("Camera");
                entity->GetComponent<TagComponent>().Group = "Cameras";
                entity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Perspective);
            }
            ImGui::Separator();
            
            if (ImGui::BeginMenu("2D Entity")) {
                if (ImGui::MenuItem("Quad")) {
                    auto entity = m_Context->CreateEntity("Quad");
                    entity->GetComponent<TagComponent>().Group = "Quads";
                    entity->AddComponent<QuadComponent>();
                }
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("3D Entity")) {
                ImGui::EndMenu();
            }

            ImGui::EndMenu(); // Create Empty Entity
        }
        ImGui::EndPopup();
    }
    
    // Delete the Entity
    if (m_EntityToBeDeleted) {
        const auto& group = m_EntityToBeDeleted->GetComponent<TagComponent>().Group;
        auto& entities = m_EntityGroups[group];
        auto pos = std::find(entities.begin(), entities.end(), m_EntityToBeDeleted);
        
        if (pos != entities.end())
            entities.erase(pos);
        
        m_Context->DestroyEntity(m_EntityToBeDeleted);
        if (m_SelectedEntity == m_EntityToBeDeleted)
            m_SelectedEntity = nullptr;
        m_EntityToBeDeleted = nullptr;
    }

    ImGui::PopID();
    ImGui::End(); // ImGui::Begin("Scene Hierarchy");

    ImGui::Begin("Entity Property");
    ImGui::PushID("Entity Property");
    
    
    
    ImGui::PopID();
    ImGui::End(); // ImGui::Begin("Scene Hierarchy");
}

/// Draw Entity node
/// @param entity entity handle
void SceneHierarchyPannel::DrawEntityTreeNode(const std::shared_ptr<Entity>& entity) {
    ImGuiTreeNodeFlags flags = ((m_SelectedEntity.get() == entity.get()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Bullet;
    auto& tag = entity->GetComponent<TagComponent>().Tag;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(*entity), flags, tag.c_str());
    if (ImGui::IsItemClicked())
        m_SelectedEntity = entity;

    // Right click of mouse
    if (ImGui::BeginPopupContextItem()) {
        m_SelectedEntity = entity;
        if (ImGui::MenuItem("Delete Entity"))
            m_EntityToBeDeleted = entity;
        
        if (ImGui::MenuItem("Duplicate Entity"))
            m_Context->DuplicateEntity(entity);
        
        ImGui::Separator();
        
        if (ImGui::BeginMenu("Add to group")) {
            for (auto [group, entities] : m_EntityGroups) {
                if (ImGui::MenuItem(group.c_str())) {
                    if (m_EntityGroups.find(entity->GetComponent<TagComponent>().Group) != m_EntityGroups.end()) {
                        // if this entity was present in some group then just remove the entiry
                        auto& entitiesInGroup = m_EntityGroups[entity->GetComponent<TagComponent>().Group];
                        auto pos = std::find(entitiesInGroup.begin(), entitiesInGroup.end(), entity);
                        if (pos != entitiesInGroup.end())
                            entitiesInGroup.erase(pos);
                    }
                    entity->GetComponent<TagComponent>().Group = group;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
    if (opened)
        ImGui::TreePop();
}

/// Grup the entity and if There is no group then render
void SceneHierarchyPannel::GroupEntities(const std::shared_ptr<Entity>& entity) {
    auto& group = entity->GetComponent<TagComponent>().Group;
    
    if (std::find(m_EntityGroups[group].begin(), m_EntityGroups[group].end(), entity) == m_EntityGroups[group].end())
        m_EntityGroups[group].emplace_back(entity);
}

void SceneHierarchyPannel::SetSelectedEntity(const std::shared_ptr<Entity>& entity) { m_SelectedEntity = entity; }
std::shared_ptr<Entity> SceneHierarchyPannel::GetSelectedEntity() const { return m_SelectedEntity; }

void SceneHierarchyPannel::SetContext(const std::shared_ptr<Scene>& context) { m_Context = context; }
std::shared_ptr<Scene> SceneHierarchyPannel::GetContext() const { return m_Context; }
