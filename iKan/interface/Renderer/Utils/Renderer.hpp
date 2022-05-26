//
//  Renderer.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

// Common Renderer File.
// Resposible to create interface between Core and Client
// ALL Renderer API to be called via this class

namespace iKan {

    /// All renderer ID type
    using RendererID = uint32_t;

    // Forward declaration
    class RendererAPI;
    class Shader;
    class Pipeline;
    
    /// Interface class to handle ALL Renderer API to be used by client
    /// Resposnible for
    /// - All Core Renderer API Wrapper (e.g. Batch Renderer, Scene Renderer ...)
    /// - Fundamental Renderer API Wrapper (e.g. Open GL Renderer API ... )
    /// - Renderer Stats API Wrapper
    class Renderer {
    public:
        /// Supported Renderer API by iKan Engine
        enum class API : uint8_t {
            None = 0,
            OpenGL = 1,
            /* Add other Supported API */};
        
        // -------------------- Core API to control Renderer -------------------------
        /// Intialize all Renderer that are supported by iKan.
        /// NOTE: to Initialize Client Specific renderer, initialize them in Client Application Constructor
        static void Init();
        
        /// Shutdown all Renderer that are supported by iKan.
        /// NOTE: to Shutdown Client Specific renderer, Shutdown them in Client Application Destructor
        static void Shutdown();
        
        /// Return the Current Set Renderer API Type
        static API GetAPI();
        /// NOTE: Set the Renerer API even before constructing window as Window is creating Renderer Context
        /// Based on the API to be set
        /// @param api Current API need to be set
        static void SetAPI(API api);
        
        // --------------- Fundamental Renderer API Wrappers -------------------------
        /// Clear all the Renderer Bits and set the Clear color to new color
        /// @param color New color of renderer Clear color
        static void Clear(const glm::vec4& color);
        /// Set the Clear color to new color
        /// @param color New color of renderer Clear color
        static void SetClearColor(const glm::vec4& color);
        /// Clear all the Renderer Bits
        static void ClearBits();
        
        /// Enable or disable Depth Field
        /// @param state flag to be set as Depth
        static void Depth(bool state);
        /// Enable or disable Blend Field
        /// @param state flag to be set as Blend
        static void Blend(bool state);
        /// Enable or disable MultiSample Field
        /// @param state flag to be set as MultiSample
        static void MultiSample(bool state);
        
        /// Update the Graphics Viewport siz
        /// @param width new width
        /// @param height new heughr
        static void SetViewportSize(float width, float height);
        
        // --------------------------- Draw API -------------------------------------
        /// Draw Vertex Array with indexed count
        /// @param pipeline pipeline ref pointer
        /// @param count count
        static void DrawIndexed(const std::shared_ptr<Pipeline>& pipeline, uint32_t count = 0);
        /// Render Complex submesh
        /// @param pipeline Pipeline
        /// @param count Count of index in submesh
        /// @param indicesdata indices data
        /// @param basevertex Base vertex pos
        static void DrawIndexedBaseVertex(const std::shared_ptr<Pipeline>& pipeline, uint32_t count, void* indicesdata, uint32_t basevertex);

        // --------------------- Renderer Stats API ---------------------------------
        /// Restet the renderer Stats each frame
        /// NOTE: Only those stats will be reset that need to be reset each frame
        /// (Draw Calls, Index Count, Vertex Count)
        static void ResetStatsEachFrame();
        /// Reste all the renderer stats
        static void ResetStats();
        /// Renderer Imgui to show renderer stats
        static void ImguiRendererStats();
        
        // ---------------- Texture Renderer API Manager ----------------------------
        /// Check if current Renderer ID is already Assigned to any Texture
        /// Return true of already present, fase if fresh ID
        /// @param rendererId Renderer ID
        static bool IsTextureRendererIDExist(RendererID rendererId);
        /// Store the Renderer ID in Renderer Set
        /// @param rendererId Renderer ID
        static void AddRendererIDs(RendererID rendererId);
        /// Remove the Renderer ID in Renderer Set
        /// @param rendererId Renderer ID
        static void RemoveRendererIDs(RendererID rendererId);
        
        // ---------------- Shader Manager ----------------------------
        static std::shared_ptr<Shader> GetShader(const std::string& path);
        
    private:
        Renderer() = default;
        MAKE_SINGLETON(Renderer);
        
        // Member varaibel
        static API s_API;
        static std::unique_ptr<RendererAPI> s_RendererAPI;
        
        // TODO: Create Texture Library for storing already Loaded Textures and their ID
        static std::set<RendererID> s_TextureRendererIDs;
    };
    
}
