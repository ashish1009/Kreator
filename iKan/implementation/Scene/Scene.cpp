//
//  Scene.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "Scene.hpp"
#include "Scene/Entity.hpp"
#include "Renderer/Utils/BatchRenderer.hpp"
#include "Renderer/Utils/TextRenderer.hpp"
#include "Renderer/Utils/Mesh.hpp"
#include "Editor/PropertyGrid.hpp"

using namespace iKan;

/// Copy the entity components
/// @param dst Destination entity
/// @param src Source entity
template<typename Component> static void CopyComponentIfExist(Entity& dst, const Entity& src) {
    if (src.HasComponent<Component>())
        dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
}

/// Create Secne Reference instance
std::shared_ptr<Scene> Scene::Create(const std::string& path) {
    return std::make_shared<Scene>();
}

/// Scene Constructor
Scene::Scene(const std::string& path) : m_FilePath(path), m_Name(StringUtils::GetNameFromFilePath(path)) {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Scene ...");
    IK_CORE_INFO("    Name : {0}", m_Name);

    m_EditorCamera = EditorCamera::Create();
    
    m_PlayTexture = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/icons/play.png"));
    m_PauseTexture = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/icons/pause.png"));
    
    if (m_State == State::Edit)
        EditScene();
    else if (m_State == State::Play)
        PlayScene();
    else
        IK_CORE_ASSERT(false, "Invalid State");
}

/// Scene Destructor
Scene::~Scene() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Scene!!!");
    IK_CORE_WARN("    Name : {0}", m_Name);
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
    CopyComponentIfExist<CameraComponent>(*newEntity.get(), *entity.get());
    CopyComponentIfExist<QuadComponent>(*newEntity.get(), *entity.get());
    CopyComponentIfExist<CircleComponent>(*newEntity.get(), *entity.get());
    // TODO: Add Duplicate Entity for other entity too later
    return newEntity;
}

/// Update the Scene
/// @param ts time step
void Scene::Update(Timestep ts) {
    m_Update(ts);
}

/// Update the Scene while editing
/// @param ts Time step
void Scene::UpdateEditor(Timestep ts) {
    m_EditorCamera->Update(ts);
    
    Render2DComponents(m_EditorCamera->GetViewProjection(), m_EditorCamera->GetViewMatrix());
    Render3DComponents(m_EditorCamera->GetPosition(), m_EditorCamera->GetViewProjection(), ts);
}

/// Update the Scene at runtime
/// @param ts Time step
void Scene::UpdateRuntime(Timestep ts) {
    if (const std::shared_ptr<Entity>& primaryCameraEntity = GetPrimaryCameraEntity(); primaryCameraEntity) {
        const std::shared_ptr<SceneCamera>& camera = primaryCameraEntity->GetComponent<CameraComponent>().Camera;
        
        const auto& tc = primaryCameraEntity->GetComponent<TransformComponent>();
        const glm::mat4& cameraTransform = tc.GetTransform();
        
        Render2DComponents(camera->GetProjectionMatrix() * glm::inverse(cameraTransform), glm::inverse(cameraTransform));
        Render3DComponents(tc.Translation, camera->GetProjectionMatrix() * glm::inverse(cameraTransform), ts);
    }
}

/// Set Scene to play mode
void Scene::PlayScene() {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Scene is Set to Play");
    
    m_State = State::Play;
    m_Update = std::bind(&Scene::UpdateRuntime, this, std::placeholders::_1);
    m_EventHandler = std::bind(&Scene::EventHandlerRuntime, this, std::placeholders::_1);
    m_RenderImgui = std::bind(&Scene::RenderImguiRuntime, this);
}

/// Set scene to Edit mode
void Scene::EditScene() {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Scene is Set to Edit");
    
    m_State = State::Edit;
    m_Update = std::bind(&Scene::UpdateEditor, this, std::placeholders::_1);
    m_EventHandler = std::bind(&Scene::EventHandlerEditor, this, std::placeholders::_1);
    m_RenderImgui = std::bind(&Scene::RenderImguiEditor, this);
}

/// Set the Selected Entity by UI
/// @param selectedEntity new selected Entity Reference pointer
void Scene::SetSelectedEntity(const std::shared_ptr<Entity>& selectedEntity) {
    m_SelectedEntity = selectedEntity;
}

/// Get the entiyt ftom Id map
/// @param id ID
const std::shared_ptr<Entity>& Scene::GetEnitityFromId(int32_t id) const {
    auto ID = m_Registry.get<IDComponent>((entt::entity)id).ID;
    return m_EntityIDMap.at(ID);
}

/// Handle the events
/// @param event event instance
void Scene::EventHandler(Event& event) {
    EventDispatcher dispatch(event);
    dispatch.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(Scene::WindowResizeEventHandler));
    m_EventHandler(event);
}

/// Handle the events while editing
/// @param event event instance
void Scene::EventHandlerEditor(Event& event) {
    m_EditorCamera->EventHandler(event);
}

/// Handle the events runtime
/// @param event event instance
void Scene::EventHandlerRuntime(Event& event) {
}

/// Render Secne Imgui
void Scene::RenderImgui() {
    m_RenderImgui();
    
    // Play Pause Buttom
    ImGui::Begin("Play/Pause", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    uint32_t texIdx = m_State == State::Edit ? m_PlayTexture->GetRendererID() : m_PauseTexture->GetRendererID();
    float size = ImGui::GetWindowHeight() - 16.0f; // 4 just random number
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
    if (PropertyGrid::ImageButton("Play/Pause", texIdx, { size, size })) {
        if (m_State == State::Edit)
            PlayScene();
        else
            EditScene();
    }
    
    ImGui::End();

}

/// Render Scene imgui while editing
void Scene::RenderImguiEditor() {
    m_EditorCamera->RendererGui();
}

/// Render Secne Imgui runtime
void Scene::RenderImguiRuntime() {
    
}

/// Update the Scene Viewport size
/// @param width new width
/// @param height new height
void Scene::SetViewport(uint32_t width, uint32_t height) {
    m_EditorCamera->SetViewportSize(width, height);
    ResizeCameraEntity(width, height);
}

/// Handle Window Resize event
/// @param event window resize event
bool Scene::WindowResizeEventHandler(WindowResizeEvent& event) {
    ResizeCameraEntity(event.GetWidth(), event.GetHeight());
    return false;
}

/// Resize the Viewport of all scene camera which are not fixed aspect ratio
void Scene::ResizeCameraEntity(uint32_t width, uint32_t height) {
    auto view = m_Registry.view<CameraComponent>();
    for (auto& entity : view) {
        auto& comp = view.get<CameraComponent>(entity);
        if (!comp.FixedAspectRatio)
            comp.Camera->SetViewportSize(width, height);
    }
}

/// Render all 2D components
/// @param viewProj Camera View projection matrix
void Scene::Render2DComponents(const glm::mat4& viewProj, const glm::mat4& view) {
    BatchRenderer::BeginBatch(viewProj, view);

    // Quads
    auto quadGroup = m_Registry.group<TransformComponent>(entt::get<QuadComponent>);
    for (const auto& entity : quadGroup) {
        const auto [transform, quadComp] = quadGroup.get<TransformComponent, QuadComponent>(entity);
        if (quadComp.Texture.Component && quadComp.Texture.Use)
            BatchRenderer::DrawQuad(transform.GetTransform(), quadComp.Texture.Component, quadComp.Color, quadComp.TilingFactor, (int32_t)entity);
        else
            BatchRenderer::DrawQuad(transform.GetTransform(), quadComp.Color, (int32_t)entity);
    }
    
    // Circles
    auto circleView = m_Registry.view<TransformComponent, CircleComponent>();
    for (const auto& entity : circleView) {
        const auto& [transform, circleComp] = circleView.get<TransformComponent, CircleComponent>(entity);
        if (circleComp.Texture.Component && circleComp.Texture.Use)
            BatchRenderer::DrawCircle(transform.GetTransform(), circleComp.Texture.Component, circleComp.Color, circleComp.TilingFactor, circleComp.Thickness, circleComp.Fade, (int32_t)entity);
        else
            BatchRenderer::DrawCircle(transform.GetTransform(), circleComp.Color, circleComp.Thickness, circleComp.Fade, (int32_t)entity);
    }
    
    // Sprites
    auto spriteView = m_Registry.view<TransformComponent, SpriteComponent>();
    for (const auto& entity : spriteView) {
        const auto& [transform, spriteComp] = spriteView.get<TransformComponent, SpriteComponent>(entity);
        if (spriteComp.SubTexture)
            BatchRenderer::DrawQuad(transform.GetTransform(), spriteComp.SubTexture, (int32_t)entity);
        else
            BatchRenderer::DrawQuad(transform.GetTransform(), glm::vec4(1.0f), (int32_t)entity); // TODO: Store color in sprite renderer Later
    }

    // Lights
    auto lightView = m_Registry.view<TransformComponent, LightComponent>();
    for (const auto& entity : lightView) {
        const auto& [transform, lightComp] = lightView.get<TransformComponent, LightComponent>(entity);
        BatchRenderer::DrawFixedViewQuad(transform.GetTransform(), m_IconTexture.PointLight, glm::vec4(lightComp.Light->Radiance.x, lightComp.Light->Radiance.y, lightComp.Light->Radiance.z, 1.0f), 1.0f, (int32_t)entity);
    }
    
    // Camera
    auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();
    for (const auto& entity : cameraView) {
        const auto& [transform, cameraComp] = cameraView.get<TransformComponent, CameraComponent>(entity);
        BatchRenderer::DrawFixedViewQuad(transform.GetTransform(), m_IconTexture.Camera, glm::vec4(1.0f), 1.0f, (int32_t)entity);
    }
    
    BatchRenderer::EndBatch();

    // Texts
    auto textView = m_Registry.view<TransformComponent, TextComponent>();
    for (const auto& entity : textView) {
        const auto& [transform, textComp] = textView.get<TransformComponent, TextComponent>(entity);
        TextRenderer::BeginBatch(viewProj);
        TextRenderer::RenderText(textComp.Text, transform.GetTransform(), textComp.Color, (uint32_t)entity);
    }
    
}

/// Render all 3D components
void Scene::Render3DComponents(const glm::vec3& cameraosition, const glm::mat4& cameraViewProj, Timestep ts) {
    auto meshView = m_Registry.view<TransformComponent, MeshComponent>();
    for (const auto& entity : meshView) {
        const auto& [transform, mesh] = meshView.get<TransformComponent, MeshComponent>(entity);
        // TODO: Move to Scene Renderer Later
        if (mesh.Mesh) {
            // Environment
            {
                auto& material = mesh.Mesh->GetBaseMaterial();
                
                // Camera 
                material->Set("u_ViewProjection", cameraViewProj);
                material->Set("u_CameraPosition", cameraosition);
                
                // Lights
                auto view = m_Registry.view<LightComponent>();
                uint32_t numLight = 0;
                SceneLight lights[MAX_LIGHT];
                for (auto entity : view) {
                    auto& comp = view.get<LightComponent>(entity);
                    lights[numLight] = *comp.Light.get();

                    if (numLight++ > MAX_LIGHT)
                        return;
                }
                
                material->Set(std::string("u_Light"), lights);
            }

            mesh.Mesh->Update(ts);
            mesh.Mesh->Draw(transform.GetTransform());
        }
    }
}

/// Get the first primary camera in the scene
std::shared_ptr<Entity> Scene::GetPrimaryCameraEntity() {
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view) {
        auto& comp = view.get<CameraComponent>(entity);
        if (comp.Primary)
            return m_EntityIDMap[m_Registry.get<IDComponent>(entity).ID];
    }
    return nullptr;
}

/// Update Scene File path and name
/// @param filepath file path
void Scene::SetFilePath(const std::string& filepath) {
    m_FilePath = filepath;
    m_Name = StringUtils::GetNameFromFilePath(m_FilePath);
}

const std::string& Scene::GetName() const { return m_Name; }
const std::string& Scene::GetFilePath() const { return m_FilePath; }

uint32_t Scene::GetNumEntities() const { return m_NumEntities; }
uint32_t Scene::GetMaxEntityId() const { return m_MaxEntityId; }

Scene::State Scene::GetState() const { return m_State; }
void Scene::SetState(Scene::State state) { m_State = state; }
bool Scene::IsEditing() const { return m_State == Scene::State::Edit; }

const std::shared_ptr<EditorCamera>& Scene::GetEditorCamera() const { return m_EditorCamera; }
