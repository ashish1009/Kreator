//
//  iKanHeaders.h
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

// To remove documentation warning in entt::entity
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wformat-security"

// Include any core headers and Macros here that should be included in client side.
// Client should include just one file to compile Core engine library

// C++ Files
#include <iostream>
#include <set>
#include <fstream>
#include <unordered_set>

// Imgui file
#include <imgui.h>

// glm math library
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// Preprocessor
#include <iKanCommon.h>

// Core Files
#include <Core/Debug/Logger.hpp>
#include <Core/Debug/Profiler.hpp>

#include <Core/Utils/Asserts.h>
#include <Core/Utils/Buffer.hpp>
#include <Core/Utils/AssetManager.hpp>
#include <Core/Utils/StringUtils.hpp>

#include <Core/Math/AABB.hpp>

#include <Core/Event/ApplicationEvent.h>
#include <Core/Event/KeyEvent.h>
#include <Core/Event/MouseEvent.h>

#include <Core/Application.hpp>
#include <Core/Layer.hpp>
#include <Core/Input.hpp>

// Renderer Files
#include <Renderer/Utils/Renderer.hpp>
#include <Renderer/Utils/Camera.hpp>
#include <Renderer/Utils/BatchRenderer.hpp>
#include <Renderer/Utils/Mesh.hpp>
#include <Renderer/Graphics/Pipeline.hpp>
#include <Renderer/Graphics/Buffers.hpp>
#include <Renderer/Graphics/Texture.hpp>
#include <Renderer/Graphics/Shader.hpp>
#include <Renderer/Graphics/FrameBuffer.hpp>

// Scene Files
#include <Scene/SceneCamera.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Entity.hpp>
#include <Scene/Component.hpp>
#include <Scene/Viewport.hpp>

// Editor Files
#include <Editor/EditorCamera.hpp>
#include <Editor/PropertyGrid.hpp>
#include <Editor/SceneHierarchyPannel.hpp>
#include <Editor/ContentBrowserPannel.hpp>

// Gui Files
#include <Gui/ImguiAPI.hpp>
#include <ImGuizmo.h>
