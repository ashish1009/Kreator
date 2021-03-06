//
//  KreatorApp.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include <iKanHeaders.h>
#include "RendererLayer.hpp"

using namespace iKan;

/// Application for rendering all the layer. Derived from Core Kreator::Application
class KreatorApp : public Application {
public:
    /// Kreator Application instance
    /// @param spec Core application specification
    KreatorApp(const Application::Specification& spec) : Application(spec) {
        IK_INFO("Creating Kreator Application ...");
        m_RendererLayer = std::make_shared<RendererLayer>();
        PushLayer(m_RendererLayer);
    }
    
    /// Kreator Application Destructor
    virtual ~KreatorApp() {
        IK_WARN("Destroying Kreator Application !!!");
    }
private:
    std::shared_ptr<Layer> m_RendererLayer;
};

/// Implementation for creating instance of Core::Application
std::unique_ptr<Application> iKan::CreateApplication() {
    Application::Specification specification;
    specification.Name = "Kreator";
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

    auto app = std::make_unique<KreatorApp>(specification);
    return app;
}
