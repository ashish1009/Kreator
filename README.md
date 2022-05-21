# iKan
A Workspace to develop iKan Engine (Graphics Engine) designed in C++ for MAC OS. Developing an Editor(Kreator) by using interface of iKan Engine 

![](/Resources/Branding/iKan.png)

## Getting Started
XCode 13 is recommended

You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/ashish1009/iKan`

Make sure that you do a `--recursive` clone to fetch all of the submodules!

## Road Map
### Temp Road Map
- Renderer Stats
- Batch Renderer (Quad and Circle)
    - Buffers
    - Pipeline
    - Shader Compiler
    - Shader Uniform
    - Texture

- Fundamental Renderer
    - Interface and Implementation for Shader Compiler.
    - Interface and Implementation for storing Renderer Buffers and Arrays.
    - Interface and Implementation for Frame buffer
    - Interface and Implementation for Loading Texture file (using stb image files)

- Renderer APIs
    - Batch Renderer API for rendering Quad and Circles (2D Batch)
    - Scene Renderer
        - API for Environment
        - Render Mesh

- Environment
    - Light
    - PBR
    - IBL
    - Shadow
    - HDR Environment Map
    - Manager to manage all Environment
    
- Mesh
    - Class to load and Render Model
    - Skeleton
    - Save All Animations
    - AABB
    
- Editor
    - Scene Hierarchy pannel : To Grow With Time
    - Added Editor 3D camera
    - Property Grid API For IM GUI
    - Content Browser pannel : To Optimize With Time
    - ImGuimzo
        - Mouse selectable entity

- Materials
    - Added Shader uniform class
    - Added Material class to handle Shader uniforms as collective buffers
    
- ECS
    - Entity Component System to make User friendly UI
    - Entity class to store the component of entity
    - Components : To Grow With Time
    - Scene class to manage all entities : To Grow With Time
    
- Project
    - Serialise and deserialise the scene : To Graw With Time

## Features
- Fundamentals
    - Logger Framework using SPD log module. API created for both core and client.
    - Assert API created for Trigger Exception in both core and client.
    - Basic Application Class created
    - Layer Base class (Client layer should be derived fom this base class)
    - Layer stack manager to store all the layers in stack
    - Scope/Function profiler Feature addded
    
- Window
    - Window create feature using GLFW library
    
- Event Handler
    - Event Abstracetion class
    - Input Handler
    
- Fundamental Renderer
    - Interface and Implementation for Renderer Context for attching Renderer to Window

## How to Use the library

### Library
`{...} directory where Kreator Engine is cloned`
Link the Kreator.a in your project... 

### Frameworks
Link following frameworks to your project...
`Cocoa.framework`
`IOKit.framework`
`OpenGL.framework`

### Header Search Paths
Add the following header search paths 
`{...} directory where Kreator Engine is cloned`
- {...}/interface
- {...}/vendors/spdlog/include
- {...}/vendors/glm
- {...}/vendors/gui/gui

### Library Search Paths
Add the following library search paths 
`{...} directory where Kreator Engine is cloned`

### Preprocesor
- Add `IK_DEBUG_FEATURE` to enable Kreator Debug features like Profiler and so on...
- Add `IK_ENABLE_LOG` to enable Logging

### Entiry point
- Include Kreator Entry point to use Creator Pre defined Application.
    - Kreator Entry point path : `{...}/interface/Entrypoint.h`
- else define your own entry point

#### If included Kreator Entry point. 
- Define `std::unique_ptr<Kreator::Application> Kreator::CreateApplication()` at client side to instanciate client Applicaiton which should be derived from Core `Kreator::Application`.
- Push Layers in Application. Base Layer is defined in Kreator. Client Layer should be derived from `Kreator::Layer`. Push using Application API `PushLayer(Layer*)`

## Basic APIs

## TODOs
- TODO: Remove Creator and Implementation files in final commit. Only Interface and Library .a file
- TODO: In Logger::Init() : Change default argument file path when Asset manager will get introduced

###Temp TODO
- Make Layer stack as Common stack and move to Utils
- Make some Macoro do delete Copy and Move Constructor and = operator for singleton class
