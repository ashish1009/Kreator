//
//  SceneHierarchyPannel.hpp
//  iKan
//
//  Created by iKan on 03/06/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    class Scene;
    class Texture;
    class Entity;
    
    /// A wrapper only to render Active Scene's Information in Imgui
    class SceneHierarchyPannel {
    public:
        const std::shared_ptr<Texture> c_EditTexture    = Renderer::GetTexture(AssetManager::GetCoreAsset("textures/icons/edit.png"));
        const std::shared_ptr<Texture> c_AddTexture     = Renderer::GetTexture(AssetManager::GetCoreAsset("textures/icons/add.png"));
        const std::shared_ptr<Texture> c_DefaultTexture = Renderer::GetTexture(AssetManager::GetCoreAsset("textures/default/noTexture.png"));

        /// Construct Scene Hierarchy pannel
        /// @param context Scene context pointer
        SceneHierarchyPannel(const std::shared_ptr<Scene>& context);
        /// Destroy Scene Hierarchy panne;
        ~SceneHierarchyPannel();
        
        /// Render Imgui
        void RenderImgui();
        
        /// Update the Selected Entity
        /// @param entity current entity pointer reference
        void SetSelectedEntity(const std::shared_ptr<Entity>& entity);
        /// Return the Selected Entity
        std::shared_ptr<Entity> GetSelectedEntity() const;
        
        /// Update the context runtime
        /// @param context Scene context
        void SetContext(const std::shared_ptr<Scene>& context);
        /// Return the context of Scene
        std::shared_ptr<Scene> GetContext() const;
        
        /// Create instance of Scene Hierarchy Pannel
        /// @param context Scene context pointer
        static std::shared_ptr<SceneHierarchyPannel> Create(const std::shared_ptr<Scene>& context);

    private:
        // Member functions
        void DrawEntityTreeNode(const std::shared_ptr<Entity>& entity);
        void GroupEntities(const std::shared_ptr<Entity>& entity);
        void DrawComponents();
        void AddComponent();
        
        // Member variables
        std::shared_ptr<Entity> m_SelectedEntity, m_EntityToBeDeleted;
        std::shared_ptr<Scene> m_Context;
        std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>> m_EntityGroups;
    };
    
}
