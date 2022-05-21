//
//  BatchRenderer.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "BatchRenderer.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/Buffer.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Texture.hpp"

using namespace iKan;

// forward declaration
class QuadData;

/// Common 2D Batch Renderer Data
/// Base class for all 2D Renderer like Quad, Circle ...
struct RendererData {
    /// Renderer Data storage
    std::shared_ptr<Pipeline> Pipeline;
    std::shared_ptr<VertexBuffer> VertexBuffer;
    std::shared_ptr<Shader> Shader;

    /// Count of Indices to be renderer in Single Batch
    uint32_t IndexCount = 0;
    
    /// Stores all the 16 Texture in array so that there is no need to load texture each frame
    /// NOTE: Load only if new texture is added or older replaced with new
    std::array<std::shared_ptr<Texture>, MaxTextureSlotsInShader> TextureSlots;
    
    /// Texture Slot index sent to Shader to render a specific Texture from slots
    /// Slot 0 is reserved for white texture (No Image only color)
    uint32_t TextureSlotIndex = 1; // 0 = white texture
    
    /// Virtual Destructor
    virtual ~RendererData() = default;
    
    friend class QuadData;
    
private:
    RendererData() = default;
 };

/// Batch Data to render Quads
struct QuadData : RendererData {
    /// Single vertex of a Quad
    struct Vertex {
        glm::vec3 Position;  // Position of a Quad
        glm::vec4 Color;     // Color of a Quad
        glm::vec2 TexCoords; // Texture coordinates of a Quad
        
        float TexIndex;     // Texture Index (Slot index which texture need to be loaded/rendered)
        float TilingFactor; // Tiling factor (Multiply Texture Tile by factor)

        int32_t ObjectID; // Pixel ID of Quad
    };
    
    // --------------- Constants -----------------
    static constexpr uint32_t VertexForSingleQuad = 4;
    static constexpr uint32_t IndicesForSingleQuad = 6;
    /// Max number of Quad to be rendered in single Batch
    /// NOTE: Memory will be reserved in GPU for MaxQuads.
    /// TODO: Make configurable in run time and While initializing the Batch Renderer
    static constexpr uint32_t MaxQuad = 1000;

    // -------------- Variables ------------------
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* VertexBufferBase = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* VertexBufferPtr = nullptr;

    /// Constructor
    QuadData() {
        IK_CORE_INFO("Creating QuadData instance ...");
    }
    /// Destructor
    virtual ~QuadData() {
        IK_CORE_WARN("Destroying QuadData instance and clearing the data !!!");
        delete [] VertexBufferBase;
        VertexBufferBase = nullptr;
    }
    
    /// start new batch for quad rendering
    void StartBatch() {
        IndexCount = 0;
        VertexBufferPtr = VertexBufferBase;
        TextureSlotIndex = 1;
    }
};
static QuadData* s_QuadData;

/// Initialize the Batch renderer for 2D Renderer
void BatchRenderer::Init() {
    IK_CORE_INFO("Initialising the Batch Renderer 2D ...");
    InitQuadData();
}

/// SHutdown or destroy the batch Renderer
void BatchRenderer::Shutdown() {
    PROFILE();
    IK_CORE_WARN("Shutting down the Batch Renderer 2D !!!");
    if (s_QuadData)
        delete s_QuadData;
}

/// Initialize Quad Data
void BatchRenderer::InitQuadData() {
    PROFILE();
    IK_CORE_INFO("    Initialising the Quad Renderer");
    s_QuadData = new QuadData();
    
    // Create Pipeline instance
    s_QuadData->Pipeline = Pipeline::Create();

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("    Quad Renderer Data");
    IK_CORE_INFO("        Max Quads per Batch     : {0}", QuadData::MaxQuad);
    IK_CORE_INFO("        Max Texture Slots Batch : {0}", MaxTextureSlotsInShader);
    IK_LOG_SEPARATOR();
}
