//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "ChessRendererLayer.hpp"

using namespace Chess;

/// Chess Renderer Layer Constructor
ChessRendererLayer::ChessRendererLayer() : Layer("Chess Renderer Layer") {
    IK_INFO("Creating '{0}' ...", m_Name);
}

/// Chess Renderer Layer Destructor
ChessRendererLayer::~ChessRendererLayer() {
    IK_WARN("Destroying '{0}' !!!", m_Name);
}

/// Attach the Renderer Layer to the application
void ChessRendererLayer::Attach() {
    IK_INFO("Attaching '{0}'", m_Name);
    
    m_Scene = Scene::Create();
    m_Scene->PlayScene();
    
    // Render the Outline board of Chess
    std::shared_ptr<Entity> backgroundEntity = m_Scene->CreateEntity("Background");
    backgroundEntity->AddComponent<QuadComponent>(glm::vec4(0.5f, 0.6f, 0.8f, 1.0f));
    
    // Setup the scene Camera Entity
    m_CameraEntity = m_Scene->CreateEntity("Camera");
    m_CameraEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Orthographic);

    // Shifiting the camera and Border block as {0, 0} is our first block (bottom left) which is at the center
    auto& camPosition = m_CameraEntity->GetComponent<TransformComponent>().Translation;
    camPosition = { 3.5f, 3.5f, 0.0f };
    auto& tc = backgroundEntity->GetComponent<TransformComponent>();
    tc.Translation = { 3.5f, 3.5f, 0.0f };
    tc.Scale = { 8.5f, 8.5f, 1.0f };

    // Initialize the Block data
    // NOTE: Row Index is the Y position of Renderer Screen
    //       Column Index is the X Position of Renderer Screen
    for (uint8_t rowIdx = 0; rowIdx < MAX_ROWS; rowIdx++) {
        for (uint8_t colIdx = 0; colIdx < MAX_COLUMNS; colIdx++) {
            // Stores the Position of Blocks
            m_Blocks[colIdx][rowIdx].X = colIdx;
            m_Blocks[colIdx][rowIdx].Y = rowIdx;
            
            // Stores the Index of each block
            m_Blocks[colIdx][rowIdx].BlockIndex = rowIdx * MAX_ROWS + colIdx;
            
            // Create Entity for each Block
            std::string blockName = "Block " + std::to_string(m_Blocks[colIdx][rowIdx].BlockIndex);
            m_Blocks[colIdx][rowIdx].Entity = m_Scene->CreateEntity(blockName);
            auto& qc = m_Blocks[colIdx][rowIdx].Entity->AddComponent<QuadComponent>();
            
            // Assign color to each block alternative
            // First check alternate rows.
            if (rowIdx % 2 == 0) {
                // Then check each alternate columns
                if (m_Blocks[colIdx][rowIdx].BlockIndex % 2 == 0)
                    // If rows % 2 is 0 then first block of row is bright
                    qc.Color = { 1.0f , 1.0f, 1.0f, 1.0f };
                else
                    // else first block of row is dark
                    qc.Color = { 0.7f, 0.1f, 0.1f, 1.0f };
            }
            else {
                // Then check each alternate columns
                if (m_Blocks[colIdx][rowIdx].BlockIndex % 2 == 0)
                    // If rows % 2 is NOT 0 then first block of row is dark
                    qc.Color = { 0.7f, 0.1f, 0.1f, 1.0f };
                else
                    // else first block of row is bright
                    qc.Color = { 1.0f , 1.0f, 1.0f, 1.0f };
            }
            
            // Update the block position
            auto& blockPosition = m_Blocks[colIdx][rowIdx].Entity->GetComponent<TransformComponent>().Translation;
            blockPosition.x = m_Blocks[colIdx][rowIdx].X;
            blockPosition.y = m_Blocks[colIdx][rowIdx].Y;
        }
    }
    
    // Before starting Game loop Update the Scene viewport
    m_Scene->SetViewport(1200, 800);
}

/// Update the renderer Layer each frame
void ChessRendererLayer::Update(Timestep ts) {
    Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
    m_Scene->Update(ts);
}

/// Render ; for Renderer Layer
void ChessRendererLayer::RenderGui() {
}

/// Detach the Renderer Lyer
void ChessRendererLayer::Detach() {
    IK_WARN("Detaching '{0}'", m_Name);
}

/// Handle the Events of Renderer Layer
void ChessRendererLayer::EventHandler(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(ChessRendererLayer::OnKeyPressed));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(ChessRendererLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(ChessRendererLayer::OnWindowResize));

    m_Scene->EventHandler(event);
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool ChessRendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
    return false;
}

/// Kwy Press Event
/// @param event Key Press event handler
bool ChessRendererLayer::OnKeyPressed(KeyPressedEvent& event) {
    return false;
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool ChessRendererLayer::OnWindowResize(WindowResizeEvent& e) {
    Renderer::SetViewportSize(e.GetWidth(), e.GetHeight());
    return false;
}
