//
//  Scene.hpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#pragma once

#include <entt.hpp>
#include "Core/Event/Event.h"
#include "Editor/EditorCamera.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Scene/SceneCamera.hpp"

namespace iKan {

    class Entity;
    class SceneHierarchyPannel;
    class SceneSerializer;
    
    /// Stores all infor and data of scene
    class Scene {
    private:
        entt::registry m_Registry;
        friend class Entity;
        friend class SceneHierarchyPannel;
        friend class SceneSerializer;
    };
    
}
