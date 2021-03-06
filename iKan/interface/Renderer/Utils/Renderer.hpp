//
//  Renderer.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once
#include "Renderer/Utils/RenderCommandQueue.hpp"

// Common Renderer File.
// Resposible to create interface between Core and Client
// ALL Renderer API to be called via this class

namespace iKan {

    /// All renderer ID type
    using RendererID = uint32_t;

    // Forward declaration
    class RendererAPI;
    class Shader;
    class Texture;
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
            /* Add other Supported API */
        };
        
        /// Stores the capability of renderer
        struct Capabilities {
            std::string Vendor;
            std::string Renderer;
            std::string Version;
            
            static Capabilities& Get() {
                static Capabilities capabilities;
                return capabilities;
            }
    
        private:
            ~Capabilities() = default;
            Capabilities() = default;
            Capabilities(const Capabilities&) = delete;
            Capabilities(Capabilities&&) = delete;
            Capabilities& operator=(const Capabilities&) = delete;
            Capabilities& operator=(Capabilities&&) = delete;
        };
        
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
        
        /// Get the Pixel ID from Viewport
        /// @param mx x Pixel of Viewport
        /// @param my y Pizel of Viewport
        /// @param pixelData Pizel Value returned
        static void GetEntityIdFromPixels(int32_t mx, int32_t my, int32_t& pixelData);
        
        // --------------------------- Draw API -------------------------------------
        /// Draw Vertex Array with indexed count
        /// @param pipeline pipeline ref pointer
        /// @param count count
        static void DrawIndexed(const std::shared_ptr<Pipeline>& pipeline, uint32_t count = 0);
        /// Draw Vertex Array with indexed cound
        /// @param pipeline vertex array ref pointer
        /// @param count count
        static void DrawArrays(const std::shared_ptr<Pipeline>& pipeline, uint32_t count);
        /// Render Complex submesh
        /// @param pipeline Pipeline
        /// @param count Count of index in submesh
        /// @param indicesdata indices data
        /// @param basevertex Base vertex pos
        static void DrawIndexedBaseVertex(const std::shared_ptr<Pipeline>& pipeline, uint32_t count, void* indicesdata, uint32_t basevertex);
        
        // ------------------------ Predefined Texts ------------------------------
        /// Render the Frame rate as text
        /// @param text Text tto be rendered
        /// @param projection window projectiom
        /// @param position position of text
        /// @param scale size of text
        /// @param color color of Text
        static void RenderText(const std::string& text, const glm::mat4& projection, const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);

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
        /// Return the shader pointer from MAP. If not present then create new shader
        /// @param path path of shader
        static std::shared_ptr<Shader> GetShader(const std::string& path);
        
        // ---------------- Texture Manager ----------------------------
        /// Return the texture pointer from MAP. If not present then create new texture
        /// @param path path of texture
        /// @param minLinear flag is min filter is linear
        /// @param magLinear flag is mag filter is linear
        static std::shared_ptr<Texture> GetTexture(const std::string& path, bool minLinear = true, bool magLinear = true);
        
        // ------------------------ Renderer Command Queue API ----------------------
        static void WaitAndRender();
        
        /// Submit the renderer API in the Renderer Command Queue. Later will be executed
        /// one by one. FunT is type of funcion
        /// @param func Fucntion pointer to be added in queue
        template<typename FuncT> static void Submit(FuncT&& func) {
            auto renderCmd = [](void* ptr) {
                auto pFunc = (FuncT*)ptr;
                (*pFunc)();

                // NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
                // however some items like uniforms which contain std::strings still exist for now
                // static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
                pFunc->~FuncT();
            };
            auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
            new (storageBuffer) FuncT(std::forward<FuncT>(func));
        }

        
    private:
        Renderer() = default;
        MAKE_SINGLETON(Renderer);
        
        // Member varaibel
        static API s_API;
        static std::unique_ptr<RendererAPI> s_RendererAPI;
        static RenderCommandQueue& GetRenderCommandQueue();
    };
    
}
