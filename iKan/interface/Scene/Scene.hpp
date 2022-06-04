//
//  Scene.hpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#pragma once

#include <entt.hpp>
#include "Core/Event/ApplicationEvent.h"
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
        /// State of Scene
        enum State : uint8_t { Play = 0, Edit = 1 };

        /// Scene Constructor
        Scene(const std::string& path = "Unsaved Scene");
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
        
        /// Update the Scene each frame
        /// @param ts time step of each frame
        void Update(Timestep ts);
        
        /// Handle the events
        /// @param event event instance
        void EventHandler(Event& event);
        
        /// Render the Imgui
        void RenderImgui();
        
        /// Update the Scene Viewport size
        /// @param width new width
        /// @param height new height
        void SetViewport(uint32_t width, uint32_t height);
        
        /// Play the Scene
        void PlayScene();
        /// Edit the Scene
        void EditScene();
        
        /// Set the Selected Entity by UI
        /// @param selectedEntity new selected Entity Reference pointer
        void SetSelectedEntity(const std::shared_ptr<Entity>& selectedEntity);
        
        /// Update the File Path
        /// @param filepath new path of file
        void SetFilePath(const std::string& filepath);
        
        /// Get the Scene name
        const std::string& GetName() const;
        /// Get the Saved Scene File path
        const std::string& GetFilePath() const;

        /// Get the number of Entities stored in Scene
        uint32_t GetNumEntities() const;
        /// Get the Max Entity ID given to scene
        uint32_t GetMaxEntityId() const;
        
        /// return the Entity pointer from Entity ID Map
        /// @param id Entity ID
        const std::shared_ptr<Entity>& GetEnitityFromId(int32_t id) const;

        /// Return the current state of Scene
        State GetState() const;
        
        /// Set the State of Scebe
        /// @param state current scene
        void SetState(State state);
        
        /// check is scene is in editing state
        bool IsEditing() const;
        
        /// return the Editor camera Instance
        const std::shared_ptr<EditorCamera>& GetEditorCamera() const;
        
        /// Create Scene instance
        static std::shared_ptr<Scene> Create(const std::string& path = "Unsaved Scene");

    private:
        // Member functions
        void UpdateEditor(Timestep ts);
        void UpdateRuntime(Timestep ts);
        
        void EventHandlerEditor(Event& event);
        void EventHandlerRuntime(Event& event);
        
        void RenderImguiEditor();
        void RenderImguiRuntime();
        
        bool WindowResizeEventHandler(WindowResizeEvent& event);

        void ResizeCameraEntity(uint32_t width, uint32_t height);
        
        void Render2DComponents(const glm::mat4& viewProj);
        
        std::shared_ptr<Entity> GetPrimaryCameraEntity();

        // Registry to store all entity
        entt::registry m_Registry;

        // Store the entity ID Map
        std::unordered_map<UUID, std::shared_ptr<Entity>> m_EntityIDMap;
        
        // Selected Entity
        std::shared_ptr<Entity> m_SelectedEntity;

        // Number of Entity stored in Scene and Max ID given to Entity
        uint32_t m_NumEntities = 0, m_MaxEntityId = 0;
        
        std::shared_ptr<EditorCamera> m_EditorCamera;

        // Function pointers
        std::function<void(Timestep)> m_Update;
        std::function<void(Event&)> m_EventHandler;
        std::function<void()> m_RenderImgui;

        State m_State = State::Edit;
        
        // Utils Memebers
        std::shared_ptr<Texture> m_PlayTexture, m_PauseTexture;
        std::string m_FilePath = "Unsaved Scene", m_Name = "Unsaved Scene";

        friend class Entity;
        friend class SceneHierarchyPannel;
        friend class SceneSerializer;
    };
    
}
