//
//  KreatorApp.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "ChessApp.hpp"

/// Implementation for creating instance of Core::Application
std::unique_ptr<Application> iKan::CreateApplication() {
    Application::Specification specification;
    specification.Name = "Chess";
    specification.Os = OS::Mac;
    specification.RendererAPI = Renderer::API::OpenGL;

    specification.StartMaximized = false;
    specification.Resizable = true;
    specification.EnableGui = true;
    
    specification.CoreAssetPath = "../../iKan/assets/";
    specification.ClientAssetPath = "../../Chess/assets/";

    // Window Specification
    specification.WindowSpec.Title = "Chess";
    specification.WindowSpec.Width = 1200;
    specification.WindowSpec.Height = 800;
    
    specification.WindowSpec.VSync = true;
    specification.WindowSpec.Decorated = true;
    specification.WindowSpec.Fullscreen = false;

    return std::make_unique<Chess::ChessApp>(specification);
}

using namespace Chess;

/// Chess Application instance
/// @param spec Core application specification
ChessApp::ChessApp(const Application::Specification& spec) : Application(spec) {
    IK_INFO("Creating Chess Application ...");
    m_RendererLayer = std::make_shared<ChessLayer>("Player 1", "Player 2");
    PushLayer(m_RendererLayer);
}

/// Chess Application Destructor
ChessApp::~ChessApp() {
    IK_WARN("Destroying Chess Application !!!");
}
