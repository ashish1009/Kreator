//
//  SceneHierarchyPannel.cpp
//  iKan
//
//  Created by iKan on 03/06/22.
//

#include "SceneHierarchyPannel.hpp"

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
    IK_CORE_INFO("Creating Scene Hierarchy pannel ...");
}

/// Scene Hierarchy pannel Destructir
SceneHierarchyPannel::~SceneHierarchyPannel() {
    IK_CORE_WARN("Destroying Scene Hierarchy pannel !!!");
}

/// Render Scene Hierarchy pannel imgui to show scene Info
void SceneHierarchyPannel::RenderImgui() {
}

void SceneHierarchyPannel::SetContext(const std::shared_ptr<Scene>& context) { m_Context = context; }
std::shared_ptr<Scene> SceneHierarchyPannel::GetContext() const { return m_Context; }
