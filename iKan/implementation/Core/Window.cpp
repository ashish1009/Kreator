//
//  Window.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "Window.hpp"
#include "Platform/MacOS/MacWindow.hpp"

using namespace iKan;

/// Copy Constructor Window Specification
/// @param other Specificaion reference
Window::Specification::Specification(const Window::Specification& other) {
    IK_CORE_INFO("Copying Window::Specification ... ");
    
    Title = other.Title;
    Width = other.Width;
    Height = other.Height;

    VSync = other.VSync;
    Decorated = other.Decorated;
    Fullscreen = other.Fullscreen;
    
    // NOTE: For now this copy constructor is only to Avoid Redundant copying the data
}

/// Create Instance of window according to enabled Platform
/// @param os Operatin system to be used. Use OS enum to check supported OS by iKan
/// @param spec Window Spcification Data
std::unique_ptr<Window> Window::Create(OS Os, const Window::Specification& prop) {
    switch (Os) {
        case OS::Mac : return std::make_unique<MacWindow>(prop);
        case OS::None:
        default: IK_CORE_ASSERT(false, "Provided OS Not Suported !!!");
    }
}
