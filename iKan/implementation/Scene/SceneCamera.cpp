//
//  SceneCamera.cpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#include "SceneCamera.hpp"
#include "Editor/PropertyGrid.hpp"

using namespace iKan;

#ifdef IK_DEBUG_FEATURE
/// return the Projection type in string
/// @param projType Projection type
static std::string GetProjectionNameFromType(SceneCamera::ProjectionType projType) {
    switch (projType) {
        case SceneCamera::ProjectionType::Orthographic: return "Orthographic";
        case SceneCamera::ProjectionType::Perspective: return "Perspective";
        default: IK_CORE_ASSERT(false, "Invalid Projection Type");
    }
}
#endif

/// Create instance of Scene Camera
/// @param projType projection type
std::shared_ptr<SceneCamera> SceneCamera::Create(ProjectionType projType) {
    return std::make_shared<SceneCamera>(projType);
}

/// Scene Camera Constructor
/// @param projType Projection type of camera
SceneCamera::SceneCamera(ProjectionType projType) : m_ProjectionType(projType) {
    PROFILE();
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Scene Camera ...");
    IK_CORE_INFO("    Projection Type : {0}", GetProjectionNameFromType(m_ProjectionType));

    if (m_ProjectionType == ProjectionType::Orthographic) {
        m_Far = 1.0f;
        m_Near = -1.0f;
        
        IK_CORE_INFO("    Size            : {0}", m_OrthographicSize);
        IK_CORE_INFO("    Aspect Ratio    : {0}", m_AspectRatio);
        IK_CORE_INFO("    Near Plane      : {0}", m_Near);
        IK_CORE_INFO("    Far Plane       : {0}", m_Far);
    }
    else if (m_ProjectionType == ProjectionType::Perspective) {
        m_Far = 10000.0f;
        m_Near = 0.01f;
        
        IK_CORE_INFO("    FOV             : {0}", m_PerspectiveFOV);
        IK_CORE_INFO("    Aspect Ratio    : {0}", m_AspectRatio);
        IK_CORE_INFO("    Near Plane      : {0}", m_Near);
        IK_CORE_INFO("    Far Plane       : {0}", m_Far);
    }
    else {
        IK_CORE_ASSERT(false, "Invalid Projection Type");
    }
    RecalculateProjection();
}

/// Scene Camera Copy Constructor
/// @param other other
SceneCamera::SceneCamera(const SceneCamera& other) {
    IK_CORE_INFO("Copying Scene Camera");
}

/// Scene Camera Destructor
SceneCamera::~SceneCamera() {
    PROFILE();
    
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Scene Camera !!!");
    IK_CORE_WARN("    Projection Type : {0}", GetProjectionNameFromType(m_ProjectionType));

    if (m_ProjectionType == ProjectionType::Orthographic) {
        IK_CORE_WARN("    Size            : {0}", m_OrthographicSize);
        IK_CORE_WARN("    Aspect Ratio    : {0}", m_AspectRatio);
        IK_CORE_WARN("    Near Plane      : {0}", m_Near);
        IK_CORE_WARN("    Far Plane       : {0}", m_Far);
    }
    else if (m_ProjectionType == ProjectionType::Perspective) {
        IK_CORE_WARN("    FOV             : {0}", m_PerspectiveFOV);
        IK_CORE_WARN("    Aspect Ratio    : {0}", m_AspectRatio);
        IK_CORE_WARN("    Near Plane      : {0}", m_Near);
        IK_CORE_WARN("    Far Plane       : {0}", m_Far);
    }
    else {
        IK_CORE_ASSERT(false, "Invalid Projection Type");
    }
}

/// Render Imgui pannel for Scene Camera
/// @param isTitlePredefined Check is there predefined title present. (if true means called this function between Imgui::Begin and Imgui::End)
/// @param pIsOpen check is window to be open or close (valid only if isTitlePredefined is false)
void SceneCamera::RenderImgui(bool isTitlePredefined, bool* pIsOpen) {
    if (!isTitlePredefined) {
        ImGui::Begin("Scene Camera");
        ImGui::PushID("Scene Camera");
    }
    
    ProjectionType newProjType = ProjectionType(PropertyGrid::ComboDrop("Projection Type", { "Perspective" , "Orthographic" }, (uint32_t)m_ProjectionType));
    if (newProjType != m_ProjectionType)
        SetProjectionType(newProjType);
    
    if (m_ProjectionType == SceneCamera::ProjectionType::Perspective) {
        float fov = glm::degrees(m_PerspectiveFOV);
        if (PropertyGrid::Float1("Vertical FOV", fov, nullptr, 1.0f, 45.0f))
            SetPerspectiveFOV(glm::radians(fov));
#if 0
        if (PropertyGrid::Float1("Near", m_Near, nullptr, 0.01f, 0.01f))
            RecalculateProjection();
        
        if (PropertyGrid::Float1("Far", m_Far, nullptr, 1.0f, 10000.0f))
            RecalculateProjection();
#endif
    }
    else if (m_ProjectionType == SceneCamera::ProjectionType::Orthographic) {
        if (PropertyGrid::Float1("Size", m_OrthographicSize, nullptr, 1.0f, 10.0f))
            RecalculateProjection();
        
#if 0
        if (PropertyGrid::Float1("Near", m_Near, nullptr, 0.1f, -1.0f))
            RecalculateProjection();
        
        if (PropertyGrid::Float1("Far", m_Far, nullptr, 0.1f, 1.0f))
            RecalculateProjection();
#endif
    }
    else {
        IK_CORE_ASSERT(false, "Invalid Projection Type");
    }
    ImGui::Separator();
    
    if (!isTitlePredefined) {
        ImGui::PopID();
        ImGui::End();
    }
}

/// Overriden Method to update Near plane
/// @param near new near value
void SceneCamera::SetNear(float near) {
    m_Near = near;
    IK_CORE_INFO("New Near Plane for Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", m_Near);
    RecalculateProjection();
}

/// Overriden Method to update Far plane
/// @param near new far value
void SceneCamera::SetFar(float far) {
    m_Far = far;
    IK_CORE_INFO("New Far Plane for Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", m_Far);
    RecalculateProjection();
}

/// Update Orthopgraphic Projection Matrix
/// @param size orthographic size of camerA
/// @param nearClip near clip
/// @param farClip far clip
void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
    m_ProjectionType = ProjectionType::Orthographic;
    m_OrthographicSize = size;
    m_Near = nearClip;
    m_Far = farClip;

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Updating Orthograpic Data of Scene Camera");
    IK_CORE_INFO("    Size            : {0}", m_OrthographicSize);
    IK_CORE_INFO("    Aspect Ratio    : {0}", m_AspectRatio);
    IK_CORE_INFO("    Near Plane      : {0}", m_Near);
    IK_CORE_INFO("    Far Plane       : {0}", m_Far);
    IK_CORE_INFO("Updating View Porjection Matrix");
    
    RecalculateProjection();
}

/// Update Orthopgraphic Projection Matrix
/// @param fov perspective fov of camerA
/// @param nearClip near clip
/// @param farClip far clip
void SceneCamera::SetPerspective(float fov, float nearClip, float farClip) {
    m_ProjectionType = ProjectionType::Perspective;
    m_PerspectiveFOV = fov;
    m_Near = nearClip;
    m_Far = farClip;
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Updating Perspective Data of Scene Camera");
    IK_CORE_INFO("    FOV             : {0}", m_PerspectiveFOV);
    IK_CORE_INFO("    Aspect Ratio    : {0}", m_AspectRatio);
    IK_CORE_INFO("    Near Plane      : {0}", m_Near);
    IK_CORE_INFO("    Far Plane       : {0}", m_Far);
    IK_CORE_INFO("Updating View Porjection Matrix");
    
    RecalculateProjection();
}

/// Set the Viewport size of cameraq
/// @param width width
/// @param height height
void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Changing Viewport Size of Scene Camera");
    IK_CORE_INFO("    Width        : {0}", width);
    IK_CORE_INFO("    Height       : {0}", height);
    IK_CORE_INFO("    Aspect Ratio : {0}", m_AspectRatio);
    RecalculateProjection();
}

/// Re calculate the Projection View Matrix
void SceneCamera::RecalculateProjection() {
    if (m_ProjectionType == ProjectionType::Perspective) {
        m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_Near, m_Far);
    }
    else if (m_ProjectionType == ProjectionType::Orthographic) {
        float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoBottom = -m_OrthographicSize * 0.5f;
        float orthoTop = m_OrthographicSize * 0.5f;
        
        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_Near, m_Far);
    }
    else {
        IK_CORE_ASSERT(false, "Invalid Projection Type");
    }
}

/// Getters
SceneCamera::ProjectionType SceneCamera::GetProjectionType() const { return m_ProjectionType; }
float SceneCamera::GetOrthographicSize() const { return m_OrthographicSize; }
float SceneCamera::GetPerspectiveFOV() const { return m_PerspectiveFOV; }

/// Setters
void SceneCamera::SetProjectionType(SceneCamera::ProjectionType type) {
    IK_CORE_INFO("Changing Projection Type of Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", GetProjectionNameFromType(m_ProjectionType));

    m_ProjectionType = type;
    if (m_ProjectionType == ProjectionType::Orthographic) {
        m_Far = 1.0f;
        m_Near = -1.0f;
    }
    else if (m_ProjectionType == ProjectionType::Perspective) {
        m_Far = 10000.0f;
        m_Near = 0.01f;
    }
    else {
        IK_CORE_ASSERT(false, "Invalid Projection Type");
    }
    RecalculateProjection();
}

void SceneCamera::SetOrthographicSize(float size) {
    IK_CORE_INFO("Changing Orthographic Size of Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", size);
    m_OrthographicSize = size;
    RecalculateProjection();
}

void SceneCamera::SetPerspectiveFOV(float fov) {
    IK_CORE_INFO("Changing Perspective FOV of Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", fov);
    m_PerspectiveFOV = fov;
    RecalculateProjection();
}
