//
//  Input.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "Input.hpp"
#include "Core/Application.hpp"
#include <GLFW/glfw3.h>

using namespace iKan;

/// Pool the key pressed input
/// @param key Key code as GLFW
bool Input::IsKeyPressed(KeyCode key) {
    auto window  = static_cast<GLFWwindow*>(Application::Get().GetWindowPtr());
    auto state   = glfwGetKey(window, static_cast<int32_t>(key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

/// Pool the mouse button pressed input
/// @param key Mouse button code as GLFW
bool Input::IsMouseButtonPressed(MouseButton button) {
    auto window  = static_cast<GLFWwindow*>(Application::Get().GetWindowPtr());
    auto state   = glfwGetMouseButton(window, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
}

/// Pool the key released input
/// @param key Key code as GLFW
bool Input::IsKeyReleased(KeyCode key) {
    auto window  = static_cast<GLFWwindow*>(Application::Get().GetWindowPtr());
    auto state   = glfwGetKey(window, static_cast<int32_t>(key));
    return state == GLFW_RELEASE;
}

/// Pool the mouse button released input
/// @param key Mouse button code as GLFW
bool Input::IsMouseButtonReleased(MouseButton button) {
    auto window  = static_cast<GLFWwindow*>(Application::Get().GetWindowPtr());
    auto state   = glfwGetMouseButton(window, static_cast<int32_t>(button));
    return state == GLFW_RELEASE;
}

/// Get the mouse position
std::pair<float, float> Input::GetMousePosition() {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindowPtr());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    return { (float)xpos, (float)ypos };
}

/// get the x position of mouse
float Input::GetMouseX() {
    auto[x, y] = GetMousePosition();
    return x;
}

/// get the y position of mouse
float Input::GetMouseY() {
    auto[x, y] = GetMousePosition();
    return y;
}
