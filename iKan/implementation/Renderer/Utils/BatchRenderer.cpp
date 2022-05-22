//
//  BatchRenderer.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "BatchRenderer.hpp"
#include "Renderer/Utils/RendererStats.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/Buffers.hpp"
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
    /// Max number of Quad to be rendered in single Batch
    /// NOTE: Memory will be reserved in GPU for MaxQuads.
    /// TODO: Make configurable in run time and While initializing the Batch Renderer
    static constexpr uint32_t MaxQuad = 1000;

    // Fixed Constants
    static constexpr uint32_t VertexForSingleQuad = 4;
    static constexpr uint32_t IndicesForSingleQuad = 6;
    static constexpr uint32_t MaxVertex = MaxQuad * VertexForSingleQuad;
    static constexpr uint32_t MaxIndices = MaxQuad * IndicesForSingleQuad;

    // -------------- Variables ------------------
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* VertexBufferBase = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* VertexBufferPtr = nullptr;
    
    /// Basic vertex of quad
    glm::vec4 VertexPositions[4];

    /// Constructor
    QuadData() {
        IK_CORE_INFO("Creating QuadData instance ...");
    }
    /// Destructor
    virtual ~QuadData() {
        IK_CORE_WARN("Destroying QuadData instance and clearing the data !!!");
        delete [] VertexBufferBase;
        VertexBufferBase = nullptr;
        
        RendererStatistics::Get().VertexBufferSize -= MaxVertex * sizeof(QuadData::Vertex);
        RendererStatistics::Get().IndexBufferSize -= MaxIndices * sizeof(uint32_t);
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
    
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("    Destroying the Quad Renderer Data");
    IK_CORE_WARN("        Max Quads per Batch             : {0}", QuadData::MaxQuad);
    IK_CORE_WARN("        Max Texture Slots Batch         : {0}", MaxTextureSlotsInShader);
    IK_CORE_WARN("        Memory Reserved for Vertex Data : {0} B ({1} KB) ", QuadData::MaxVertex * sizeof(QuadData::Vertex),  QuadData::MaxVertex * sizeof(QuadData::Vertex) / 1000.0f );
    IK_LOG_SEPARATOR();

    if (s_QuadData)
        delete s_QuadData;
}

/// Initialize Quad Data
void BatchRenderer::InitQuadData() {
    PROFILE();
    // Alloc memory for Quad Data
    s_QuadData = new QuadData();
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("    Initialising the Quad Renderer");
    IK_CORE_INFO("        Max Quads per Batch             : {0}", QuadData::MaxQuad);
    IK_CORE_INFO("        Max Texture Slots Batch         : {0}", MaxTextureSlotsInShader);
    IK_CORE_INFO("        Memory Reserved for Vertex Data : {0} B ({1} KB) ", QuadData::MaxVertex * sizeof(QuadData::Vertex),  QuadData::MaxVertex * sizeof(QuadData::Vertex) / 1000.0f );
    IK_LOG_SEPARATOR();

    
    // Allocating the memory for vertex Buffer Pointer
    s_QuadData->VertexBufferBase = new QuadData::Vertex[QuadData::MaxVertex];
    
    // Create Pipeline instance
    s_QuadData->Pipeline = Pipeline::Create();
    
    // Create vertes Buffer
    s_QuadData->VertexBuffer = VertexBuffer::Create(QuadData::MaxVertex * sizeof(QuadData::Vertex));
    s_QuadData->VertexBuffer->AddLayout({
        { "a_Position",     ShaderDataType::Float3 },
        { "a_Color",        ShaderDataType::Float4 },
        { "a_TexCoords",    ShaderDataType::Float2 },
        { "a_TexIndex",     ShaderDataType::Float },
        { "a_TilingFactor", ShaderDataType::Float },
        { "a_ObjectID",     ShaderDataType::Int },
    });
    s_QuadData->Pipeline->AddVertexBuffer(s_QuadData->VertexBuffer);

    // Create Index Buffer
    uint32_t* quadIndices = new uint32_t[QuadData::MaxIndices];
    
    uint32_t offset = 0;
    for (size_t i = 0; i < QuadData::MaxIndices; i += QuadData::IndicesForSingleQuad) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;
        
        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;
        
        offset += 4;
    }
    
    std::shared_ptr<IndexBuffer> ib = IndexBuffer::CreateWithCount(quadIndices, QuadData::MaxIndices);
    s_QuadData->Pipeline->SetIndexBuffer(ib);
    delete[] quadIndices;
    
    // Creating white texture for colorful quads witout any texture or sprite
    uint32_t whiteTextureData = 0xffffffff;
    s_QuadData->TextureSlots[0] = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));

    // Setting basic Vertex point of quad
    s_QuadData->VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_QuadData->VertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    s_QuadData->VertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    s_QuadData->VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}
