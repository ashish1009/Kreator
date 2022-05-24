//
//  KreatorApp.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include <iKanHeaders.h>
#include "ChessRendererLayer.hpp"

using namespace iKan;

/// Application for rendering all the layer. Derived from Core iKan::Application
class ChessApp : public Application {
public:
    /// Chess Application instance
    /// @param spec Core application specification
    ChessApp(const Application::Specification& spec) : Application(spec) {
        IK_INFO("Creating Chess Application ...");
    }
    
    /// Chess Application Destructor
    virtual ~ChessApp() {
        IK_WARN("Destroying Chess Application !!!");
    }
private:
    std::shared_ptr<Layer> m_RendererLayer;
};

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
    specification.ClientAssetPath = "../../Kreator/assets/";

    // Window Specification
    specification.WindowSpec.Title = "Kreator";
    specification.WindowSpec.Width = 1600;
    specification.WindowSpec.Height = 900;
    
    specification.WindowSpec.VSync = true;
    specification.WindowSpec.Decorated = true;
    specification.WindowSpec.Fullscreen = false;

    return std::make_unique<ChessApp>(specification);
}
