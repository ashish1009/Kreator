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
    
    // Initialize the Block data
    for (uint8_t rowIdx = 0; rowIdx < MAX_ROWS; rowIdx++) {
        for (uint8_t colIdx = 0; colIdx < MAX_COLUMNS; colIdx++) {
            // Stores the Position of Blocks
            m_Blocks[rowIdx][colIdx].X = rowIdx;
            m_Blocks[rowIdx][colIdx].Y = colIdx;
            
            // Stores the Index of each block
            m_Blocks[rowIdx][colIdx].BlockIndex = rowIdx * MAX_ROWS + colIdx;
            
            // Create Entity for each Block
            std::string blockName = "Block " + std::to_string(m_Blocks[rowIdx][colIdx].BlockIndex);
            m_Blocks[rowIdx][colIdx].Entity = m_Scene->CreateEntity(blockName);
            
            // Update the block position
            auto& blockPosition = m_Blocks[rowIdx][colIdx].Entity->GetComponent<TransformComponent>().Translation;
            blockPosition.x = rowIdx;
            blockPosition.y = colIdx;
        }
    }
}

/// Update the renderer Layer each frame
void ChessRendererLayer::Update(Timestep ts) {
    Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
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
