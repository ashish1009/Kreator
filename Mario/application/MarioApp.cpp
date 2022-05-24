//
//  KreatorApp.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include <iKanHeaders.h>
#include "MarioRendererLayer.hpp"

using namespace iKan;

/// Application for rendering all the layer. Derived from Core iKan::Application
class MarioApp : public Application {
public:
    /// MarioApp Application instance
    /// @param spec Core application specification
    MarioApp(const Application::Specification& spec) : Application(spec) {
        IK_INFO("Creating Mario Application ...");
    }
    
    /// MarioApp Application Destructor
    virtual ~MarioApp() {
        IK_WARN("Destroying Mario Application !!!");
    }
private:
    std::shared_ptr<Layer> m_RendererLayer;
};

/// Implementation for creating instance of Core::Application
std::unique_ptr<Application> iKan::CreateApplication() {
    Application::Specification specification;
    specification.Name = "Mario";
    specification.Os = OS::Mac;
    specification.RendererAPI = Renderer::API::OpenGL;

    specification.StartMaximized = false;
    specification.Resizable = true;
    specification.EnableGui = true;
    
    specification.CoreAssetPath = "../../iKan/assets/";
    specification.ClientAssetPath = "../../Mario/assets/";

    // Window Specification
    specification.WindowSpec.Title = "Mario";
    specification.WindowSpec.Width = 1600;
    specification.WindowSpec.Height = 900;
    
    specification.WindowSpec.VSync = true;
    specification.WindowSpec.Decorated = true;
    specification.WindowSpec.Fullscreen = false;

    return std::make_unique<MarioApp>(specification);
}
