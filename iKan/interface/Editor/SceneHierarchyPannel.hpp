//
//  SceneHierarchyPannel.hpp
//  iKan
//
//  Created by iKan on 03/06/22.
//

#pragma once

namespace iKan {
    
    class Scene;
    class Entity;
    
    /// A wrapper only to render Active Scene's Information in Imgui
    class SceneHierarchyPannel {
    public:
        SceneHierarchyPannel(const std::shared_ptr<Scene>& context);
        ~SceneHierarchyPannel();

        void RenderImgui();
        
        void SetContext(const std::shared_ptr<Scene>& context);
        std::shared_ptr<Scene> GetContext() const;
        
        static std::shared_ptr<SceneHierarchyPannel> Create(const std::shared_ptr<Scene>& context);

    private:
        std::shared_ptr<Scene> m_Context;
    };
    
}
