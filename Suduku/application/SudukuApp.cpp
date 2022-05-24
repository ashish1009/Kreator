//
//  KreatorApp.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include <iKanHeaders.h>
#include "SudukuRendererLayer.hpp"

using namespace iKan;

/// Application for rendering all the layer. Derived from Core Kreator::Application
class SudukuApp : public Application {
public:
    /// Suduku Application instance
    /// @param spec Core application specification
    SudukuApp(const Application::Specification& spec) : Application(spec) {
        IK_INFO("Creating Suduku Application ...");
    }
    
    /// Suduku Application Destructor
    virtual ~SudukuApp() {
        IK_WARN("Destroying Suduku Application !!!");
    }
private:
    std::shared_ptr<Layer> m_RendererLayer;
};

/// Implementation for creating instance of Core::Application
std::unique_ptr<Application> iKan::CreateApplication() {
    Application::Specification specification;
    specification.Name = "Suduku";
    specification.Os = OS::Mac;
    specification.RendererAPI = Renderer::API::OpenGL;

    specification.StartMaximized = false;
    specification.Resizable = true;
    specification.EnableGui = true;
    
    specification.CoreAssetPath = "../../iKan/assets/";
    specification.ClientAssetPath = "../../Suduku/assets/";

    // Window Specification
    specification.WindowSpec.Title = "Suduku";
    specification.WindowSpec.Width = 1600;
    specification.WindowSpec.Height = 900;
    
    specification.WindowSpec.VSync = true;
    specification.WindowSpec.Decorated = true;
    specification.WindowSpec.Fullscreen = false;

    return std::make_unique<SudukuApp>(specification);
}
