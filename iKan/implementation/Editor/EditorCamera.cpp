//
//  EditorCamera.cpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#include "EditorCamera.hpp"
#include "Core/Input.hpp"
#include "Editor/PropertyGrid.hpp"

using namespace iKan;

/// Create shared intance pointer of Editor camera
std::shared_ptr<EditorCamera> EditorCamera::Create() {
    return std::make_shared<EditorCamera>();
}

///Editor camera default constructor. Setup the default perspective view
EditorCamera::EditorCamera() {
    m_Near = 0.01f;
    m_Far = 10000.0f;
    
    m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
    
    glm::vec3 position = { -15, 5, 5};
    m_Distance = glm::distance(position, m_FocalPoint);
    
    UpdateCameraView();

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Editor Camera ... ");
    IK_CORE_INFO("    FOV             : {0}", m_FOV);
    IK_CORE_INFO("    Aspect Ratio    : {0}", m_AspectRatio);
    IK_CORE_INFO("    Near Plane      : {0}", m_Near);
    IK_CORE_INFO("    Far Plane       : {0}", m_Far);
    IK_CORE_INFO("    FocalPoint      : {0} | {1} | {2}", m_FocalPoint.x, m_FocalPoint.y, m_FocalPoint.z);
}

/// Editor camera default destructor.
EditorCamera::~EditorCamera() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Editor Camera !!!");
    IK_CORE_WARN("    FOV             : {0}", m_FOV);
    IK_CORE_WARN("    Aspect Ratio    : {0}", m_AspectRatio);
    IK_CORE_WARN("    Near Plane      : {0}", m_Near);
    IK_CORE_WARN("    Far Plane       : {0}", m_Far);
    IK_CORE_WARN("    FocalPoint      : {0} | {1} | {2}", m_FocalPoint.x, m_FocalPoint.y, m_FocalPoint.z);
}

/// Update camera each frame
/// @param ts Time difference frame
void EditorCamera::Update(Timestep ts) {
    if (Input::IsKeyPressed(KeyCode::LeftAlt)) {
        const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
        glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
        m_InitialMousePosition = mouse;
        
        if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
            MouseRotate(delta);
    }
    if (Input::IsKeyPressed(KeyCode::LeftControl)) {
        const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
        glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
        m_InitialMousePosition = mouse;
        
        if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
            MousePan(delta);
    }
    if (Input::IsKeyPressed(KeyCode::LeftSuper)) {
        const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
        glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
        m_InitialMousePosition = mouse;
        
        if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
            MouseZoom(delta.y);
    }
    
    UpdateCameraView();
}

/// Editor Camera event handler
/// @param e Event instance
void EditorCamera::EventHandler(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(IK_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(EditorCamera::OnWindowResize));
}

/// Mousescroll event
/// @param e Scroll event
bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
    float delta = e.GetYOffset() * 0.1f;
    MouseZoom(delta);
    return false;
}

/// Resize the camere
/// @param e resize event
bool EditorCamera::OnWindowResize(WindowResizeEvent& e) {
    SetViewportSize(e.GetWidth(), e.GetHeight());
    return false;
}

/// Update Camera aspect ration and projection matrix
/// @param width New width
/// @param height new height
void EditorCamera::SetViewportSize(uint32_t width, uint32_t height) {
    if (height == 0)
        return;
    
    m_ViewportWidth  = width;
    m_ViewportHeight = height;
    m_AspectRatio    = (float)m_ViewportWidth / (float)m_ViewportHeight;
    
    m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
    UpdateCameraView();
    IK_CORE_INFO("Changing Viewport Size of Editor Camera : {0} x {1}. (NOTE: Updating View Projection Matrix)", width, height);
}

/// Update Camera view Matrix
void EditorCamera::UpdateCameraView() {
    m_Position = CalculatePosition();
    glm::quat orientation = GetOrientation();
    
    m_Rotation   = glm::eulerAngles(orientation) * (180.0f / (float)M_PI);
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

/// Zoom the Editor camera using Mouse
/// @param delta Delta offset of zoom
void EditorCamera::MouseZoom(float delta) {
    m_Distance -= delta * ZoomSpeed();
    if (m_Distance < 1.0f) {
        m_FocalPoint += GetForwardDirection();
        m_Distance = 1.0f;
    }
}

/// Moue pan Shift the view left right
/// @param delta Delta step of mouse scrol
void EditorCamera::MousePan(const glm::vec2& delta) {
    auto [xSpeed, ySpeed] = PanSpeed();
    m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
    m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
}

/// Mouse rorate. Rotate the camera view
/// @param delta Delta offset of mouse move
void EditorCamera::MouseRotate(const glm::vec2& delta) {
    float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    m_Yaw   += yawSign * delta.x * RotationSpeed();
    m_Pitch += delta.y * RotationSpeed();
}

/// get pan speed.
std::pair<float, float> EditorCamera::PanSpeed() const {
    float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
    
    float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
    
    return { xFactor, yFactor };
}

/// get rotation speed
float EditorCamera::RotationSpeed() const {
    return 0.8f;
}

/// Get Mouse Zoom speed. Speed depends on the distance
float EditorCamera::ZoomSpeed() const {
    float distance  = m_Distance * 0.2f;
    distance        = std::max(distance, 0.0f);
    
    float speed = distance * distance;
    speed       = std::min(speed, 100.0f);
    
    return speed;
}

/// Render Imgui Window for Editor camera
void EditorCamera::RendererGui() {
    ImGui::Begin("Editor Camera");
    ImGui::PushID("Editor Camera");

    static float fovAngle = 75.0f;
    if (PropertyGrid::Float1("FOV", fovAngle, nullptr, 1.0f, 10.0f, 80.0f, 300.0f)) {
        m_FOV = glm::radians(fovAngle);
        m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
    }
    ImGui::Separator();
    
    PropertyGrid::Float3("Focal Point", m_FocalPoint, nullptr, 0.1f, 0.0f, 80.0f);
    PropertyGrid::Float1("Distance", m_Distance, nullptr, 0.1f, 0.0f, 80.0f);
    
#if 0
    if (PropertyGrid::Float1("Near", m_Near, nullptr, 0.01f, 0.01f, 80.0f))
        m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
        
    if (PropertyGrid::Float1("Far", m_Far, nullptr, 1.0f, 10000.0f, 80.0f))
        m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
#endif
    ImGui::PopID();
    ImGui::End();
}

/// getters
glm::vec3 EditorCamera::CalculatePosition() const { return m_FocalPoint - GetForwardDirection() * m_Distance; }
glm::quat EditorCamera::GetOrientation() const { return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f)); }

glm::mat4 EditorCamera::GetViewProjection() const { return m_Projection * m_ViewMatrix; }
const glm::mat4& EditorCamera::GetViewMatrix() const { return m_ViewMatrix; }

const glm::vec3& EditorCamera::GetPosition() const { return m_Position; }
const glm::vec3& EditorCamera::GetFocalPoint() const { return m_FocalPoint; }

glm::vec3 EditorCamera::GetUpDirection() const { return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f)); }
glm::vec3 EditorCamera::GetRightDirection() const {return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f)); }
glm::vec3 EditorCamera::GetForwardDirection() const { return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f)); }

/// Setter
void EditorCamera::SetFocalPoint(glm::vec3& newFp) { m_FocalPoint = newFp; }
