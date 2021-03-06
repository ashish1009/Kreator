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
#include "Renderer/Utils/Mesh.hpp"

using namespace iKan;

/// Draw the components in property pannel. Takes the function pointer in argument
template<typename T, typename UIFunction> static void DrawComponent(const std::string& name, const std::shared_ptr<Entity>& entity, UIFunction uiFunction) {
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen |
    ImGuiTreeNodeFlags_Framed |
    ImGuiTreeNodeFlags_SpanAvailWidth |
    ImGuiTreeNodeFlags_AllowItemOverlap |
    ImGuiTreeNodeFlags_FramePadding;
    
    if (entity->HasComponent<T>()) {
        auto& component               = entity->GetComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
        ImGui::PopStyleVar(); // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
            ImGui::OpenPopup("ComponentSettings");
        
        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;
            
            ImGui::EndPopup();
        }
        
        if (open) {
            uiFunction(component);
            ImGui::TreePop();
        }
        
        if (removeComponent)
            entity->RemoveComponent<T>();
    }
}


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
    
    // Show Scene Name
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
        if (ImGui::BeginMenu("Entity")) {
            if (ImGui::MenuItem("Empty Entity"))
                m_SelectedEntity = m_Context->CreateEntity("Empty Entity");
            ImGui::Separator();
            
            if (ImGui::MenuItem("Camera")) {
                m_SelectedEntity = m_Context->CreateEntity("Camera");
                m_SelectedEntity->GetComponent<TagComponent>().Group = "Cameras";
                m_SelectedEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Perspective);
                m_SelectedEntity->GetComponent<TransformComponent>().Scale = glm::vec3(0.5);
            } // Camera
            ImGui::Separator();
            
            if (ImGui::MenuItem("Light")) {
                m_SelectedEntity = m_Context->CreateEntity("Light");
                m_SelectedEntity->GetComponent<TagComponent>().Group = "Lights";
                m_SelectedEntity->AddComponent<LightComponent>();
                m_SelectedEntity->GetComponent<TransformComponent>().Scale = glm::vec3(0.5);
            } // Light
            ImGui::Separator();
            
            if (ImGui::BeginMenu("2D Entity")) {
                if (ImGui::MenuItem("Quad")) {
                    m_SelectedEntity = m_Context->CreateEntity("Quad");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "2D Fundamentals";
                    m_SelectedEntity->AddComponent<QuadComponent>();
                }
                if (ImGui::MenuItem("Circle")) {
                    m_SelectedEntity = m_Context->CreateEntity("Circle");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "2D Fundamentals";
                    m_SelectedEntity->AddComponent<CircleComponent>();
                }
                if (ImGui::MenuItem("Text")) {
                    m_SelectedEntity = m_Context->CreateEntity("Text");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Texts";
                    m_SelectedEntity->AddComponent<TextComponent>("Enter Text Here");
                }
                ImGui::EndMenu();
            } // 2D Entity
            
            if (ImGui::BeginMenu("3D Entity")) {
                if (ImGui::MenuItem("Empty Mesh")) {
                    m_SelectedEntity = m_Context->CreateEntity("Mesh");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Complex Meshes";
                    m_SelectedEntity->AddComponent<MeshComponent>();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Cube")) {
                    m_SelectedEntity = m_Context->CreateEntity("Cube");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Fundamental Meshes";
                    auto& mc = m_SelectedEntity->AddComponent<MeshComponent>();
                    mc.Mesh = Mesh::Create(AssetManager::GetCoreAsset("models/Cube.fbx"), (uint32_t)(*m_SelectedEntity.get()));
                }
                if (ImGui::MenuItem("Sphere")) {
                    m_SelectedEntity = m_Context->CreateEntity("Sphere");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Fundamental Meshes";
                    auto& mc = m_SelectedEntity->AddComponent<MeshComponent>();
                    mc.Mesh = Mesh::Create(AssetManager::GetCoreAsset("models/Sphere.fbx"), (uint32_t)(*m_SelectedEntity.get()));
                }
                if (ImGui::MenuItem("Plane")) {
                    m_SelectedEntity = m_Context->CreateEntity("Plane");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Fundamental Meshes";
                    auto& mc = m_SelectedEntity->AddComponent<MeshComponent>();
                    mc.Mesh = Mesh::Create(AssetManager::GetCoreAsset("models/Plane.obj"), (uint32_t)(*m_SelectedEntity.get()));
                }
                if (ImGui::MenuItem("Capsule")) {
                    m_SelectedEntity = m_Context->CreateEntity("Capsule");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Fundamental Meshes";
                    auto& mc = m_SelectedEntity->AddComponent<MeshComponent>();
                    mc.Mesh = Mesh::Create(AssetManager::GetCoreAsset("models/Capsule.fbx"), (uint32_t)(*m_SelectedEntity.get()));
                }
                if (ImGui::MenuItem("Cone")) {
                    m_SelectedEntity = m_Context->CreateEntity("Cone");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Fundamental Meshes";
                    auto& mc = m_SelectedEntity->AddComponent<MeshComponent>();
                    mc.Mesh = Mesh::Create(AssetManager::GetCoreAsset("models/Cone.fbx"), (uint32_t)(*m_SelectedEntity.get()));
                }
                if (ImGui::MenuItem("Cylinder")) {
                    m_SelectedEntity = m_Context->CreateEntity("Cylinder");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Fundamental Meshes";
                    auto& mc = m_SelectedEntity->AddComponent<MeshComponent>();
                    mc.Mesh = Mesh::Create(AssetManager::GetCoreAsset("models/Cylinder.fbx"), (uint32_t)(*m_SelectedEntity.get()));
                }
                if (ImGui::MenuItem("Torus")) {
                    m_SelectedEntity = m_Context->CreateEntity("Torus");
                    m_SelectedEntity->GetComponent<TagComponent>().Group = "Fundamental Meshes";
                    auto& mc = m_SelectedEntity->AddComponent<MeshComponent>();
                    mc.Mesh = Mesh::Create(AssetManager::GetCoreAsset("models/Torus.fbx"), (uint32_t)(*m_SelectedEntity.get()));
                }
                ImGui::EndMenu();
            } // 3D Entity
            ImGui::EndMenu();
        } // Entity
        
        if (ImGui::BeginMenu("Scene Debug")) {
            if (m_Context->m_ShowIcons) {
                if (ImGui::MenuItem("Hide Icons"))
                    m_Context->m_ShowIcons = false;
            }
            else {
                if (ImGui::MenuItem("Show Icons"))
                    m_Context->m_ShowIcons = true;
            }
            ImGui::EndPopup();
        }
        ImGui::EndPopup();
    }
    
    // Delete the Entity
    if (m_EntityToBeDeleted) {
        const auto& group = m_EntityToBeDeleted->GetComponent<TagComponent>().Group;
        auto& entities = m_EntityGroups.at(group);
        auto pos = std::find(entities.begin(), entities.end(), m_EntityToBeDeleted);
        
        if (pos != entities.end())
            entities.erase(pos);
        
        m_Context->DestroyEntity(m_EntityToBeDeleted);
        if (m_SelectedEntity == m_EntityToBeDeleted)
            m_SelectedEntity = nullptr;
        m_EntityToBeDeleted = nullptr;
    }
    
    // Remove Entity from gorup if Group is changed
    if (m_EntityToRemoveFromGroup) {
        auto& entities = m_EntityGroups.at(m_DeleteEntityFromGroup);
        auto pos = std::find(entities.begin(), entities.end(), m_EntityToRemoveFromGroup);
        
        if (pos != entities.end())
            entities.erase(pos);

        m_EntityToRemoveFromGroup = nullptr;
    }

    ImGui::PopID();
    ImGui::End(); // ImGui::Begin("Scene Hierarchy");

    ImGui::Begin("Entity Property");
    ImGui::PushID("Entity Property");
    
    if (m_SelectedEntity)
        DrawComponents();
    
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
        
        const auto& selectedEntityGroup = m_SelectedEntity->GetComponent<TagComponent>().Group;
        if (ImGui::BeginMenu("Add to group")) {
            for (auto [group, entities] : m_EntityGroups) {
                if (ImGui::MenuItem(group.c_str(), nullptr, false, group != selectedEntityGroup)) {
                    if (m_EntityGroups.find(entity->GetComponent<TagComponent>().Group) != m_EntityGroups.end()) {
                        m_DeleteEntityFromGroup = entity->GetComponent<TagComponent>().Group;
                        m_EntityToRemoveFromGroup = entity;
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

/// Render Property of the selected Entity
void SceneHierarchyPannel::DrawComponents() {
    auto& group = m_SelectedEntity->GetComponent<TagComponent>().Group;
    static std::string olderGroup = group;
    static bool groupOpened = false;
    if (groupOpened) {
        ImGui::SameLine();
        if (PropertyGrid::String(group)) {
            groupOpened = false;

            // if this entity was present in some group then just remove the entiry
            auto& entities = m_EntityGroups[olderGroup];
            auto pos = std::find(entities.begin(), entities.end(), m_SelectedEntity);

            if (pos != entities.end())
                entities.erase(pos);
        }
    }
    else {
        if (PropertyGrid::ImageButton("Group Edit", c_EditTexture->GetRendererID(), { 20.0f, 20.0f })) {
            olderGroup = group;
            groupOpened = true;
        }

        ImGui::SameLine();
        ImGui::Text(group.c_str());
        ImGui::Separator();
    }

    auto& tag = m_SelectedEntity->GetComponent<TagComponent>().Tag;
    PropertyGrid::String((std::to_string((uint32_t)*m_SelectedEntity.get())).c_str(), tag, 50.0f, 300.0f);
    ImGui::SameLine();
    
    ImGui::PushItemWidth(-1);
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.95f));
    
    if (PropertyGrid::ImageButton("Add", c_AddTexture->GetRendererID(), { 20.0f, 20.0f }))
        ImGui::OpenPopup("AddComponent");
    
    if (ImGui::BeginPopup("AddComponent")) {
        AddComponent();
        ImGui::EndPopup();
    }
    
    ImGui::PopItemWidth();
    
    DrawComponent<TransformComponent>("Transform", m_SelectedEntity, [](auto& tc) { tc.RenderImgui(); });
    DrawComponent<CameraComponent>("Camera", m_SelectedEntity, [](auto& cc) { cc.RenderImgui(); });
    DrawComponent<QuadComponent>("Quad", m_SelectedEntity, [this](auto& qc) { qc.RenderImgui(c_DefaultTexture); });
    DrawComponent<CircleComponent>("Circle", m_SelectedEntity, [this](auto& cc) { cc.RenderImgui(c_DefaultTexture); });
    DrawComponent<MeshComponent>("Mesh", m_SelectedEntity, [this](auto& mc) { mc.RenderImgui(c_DefaultTexture, m_SelectedEntity); });
    DrawComponent<TextComponent>("Text", m_SelectedEntity, [](auto& tc) { tc.RenderImgui(); });
    DrawComponent<LightComponent>("Light", m_SelectedEntity, [](auto& lc) { lc.RenderImgui(); });
}

/// Add Component
void SceneHierarchyPannel::AddComponent() {
    if (ImGui::BeginMenu("Camera")) {
        if (ImGui::MenuItem("Perspective")) {
            m_SelectedEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Perspective);
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Orthographics")) {
            m_SelectedEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Orthographic);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Light")) {
        m_SelectedEntity->AddComponent<LightComponent>();
        ImGui::CloseCurrentPopup();
    }
    ImGui::Separator();
    if (ImGui::BeginMenu("2D Element")) {
        if (ImGui::MenuItem("Quad")) {
            m_SelectedEntity->AddComponent<QuadComponent>();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Circle")) {
            m_SelectedEntity->AddComponent<CircleComponent>();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Text")) {
            m_SelectedEntity->AddComponent<TextComponent>("Enter Text Here");
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    ImGui::Separator();
    if (ImGui::BeginMenu("3D Element")) {
        if (ImGui::MenuItem("Mesh")) {
            m_SelectedEntity->AddComponent<MeshComponent>();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
}

void SceneHierarchyPannel::SetSelectedEntity(const std::shared_ptr<Entity>& entity) {
    m_SelectedEntity = entity;
    m_Context->SetSelectedEntity(entity);
}

std::shared_ptr<Entity> SceneHierarchyPannel::GetSelectedEntity() const { return m_SelectedEntity; }

void SceneHierarchyPannel::SetContext(const std::shared_ptr<Scene>& context) { m_Context = context; }
std::shared_ptr<Scene> SceneHierarchyPannel::GetContext() const { return m_Context; }
