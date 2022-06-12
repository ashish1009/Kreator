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
class CircleData;

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
    friend class CircleData;
    
private:
    RendererData() = default;
 };

/// Batch Data to Rendering Quads
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

/// Batch Data to Rendering Circles
struct CircleData : RendererData {
    /// Single vertex of a Circle
    struct Vertex {
        glm::vec3 Position;  // Position of a Quad
        glm::vec4 Color;     // Color of a Quad
        glm::vec2 TexCoords; // Texture coordinates of a Quad
        
        float TexIndex;     // Texture Index (Slot index which texture need to be loaded/rendered)
        float TilingFactor; // Tiling factor (Multiply Texture Tile by factor)
        float Thickness;    // Thickness of Circle
        float Fade;         // Fadeness of Edge of Circle
        
        int32_t ObjectID; // Pixel ID of Quad
    };
    
    // --------------- Constants -----------------
    /// Max number of Quad to be rendered in single Batch
    /// NOTE: Memory will be reserved in GPU for MaxQuads.
    /// TODO: Make configurable in run time and While initializing the Batch Renderer
    static constexpr uint32_t MaxCircles = 1000;

    // Fixed Constants
    static constexpr uint32_t VertexForSingleCircle = 4;
    static constexpr uint32_t IndicesForSingleCircle = 6;
    static constexpr uint32_t MaxVertex = MaxCircles * VertexForSingleCircle;
    static constexpr uint32_t MaxIndices = MaxCircles * IndicesForSingleCircle;

    // -------------- Variables ------------------
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* VertexBufferBase = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* VertexBufferPtr = nullptr;
    
    /// Basic vertex of quad
    /// Vertex of circle is taken as Quad only
    glm::vec4 VertexPositions[4];

    /// Constructor
    CircleData() {
        IK_CORE_INFO("Creating CircleData instance ...");
    }
    /// Destructir
    virtual ~CircleData() {
        IK_CORE_WARN("Destroying Circle Data instance and clearing the data !!!");
        delete [] VertexBufferBase;
        VertexBufferBase = nullptr;

        RendererStatistics::Get().VertexBufferSize -= CircleData::MaxVertex * sizeof(CircleData::Vertex);
        RendererStatistics::Get().IndexBufferSize -= CircleData::MaxIndices * sizeof(uint32_t);
    }
    
    /// start new batch for quad rendering
    void StartBatch() {
        IndexCount = 0;
        VertexBufferPtr = VertexBufferBase;
        TextureSlotIndex = 1;
    }
};
static CircleData* s_CircleData;

/// Initialize the Batch renderer for 2D Renderer
void BatchRenderer::Init() {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Initialising the Batch Renderer 2D ...");
    InitQuadData();
    InitCircleData();
}

/// SHutdown or destroy the batch Renderer
void BatchRenderer::Shutdown() {
    PROFILE();
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Shutting down the Batch Renderer 2D !!!");
    
    IK_CORE_WARN("    Destroying the Quad Renderer Data");
    IK_CORE_WARN("        Max Quads per Batch             : {0}", QuadData::MaxQuad);
    IK_CORE_WARN("        Max Texture Slots Batch         : {0}", MaxTextureSlotsInShader);
    IK_CORE_WARN("        Memory Reserved for Vertex Data : {0} B ({1} KB) ", QuadData::MaxVertex * sizeof(QuadData::Vertex),  QuadData::MaxVertex * sizeof(QuadData::Vertex) / 1000.0f );

    IK_CORE_WARN("    Destroying the Circle Renderer Data");
    IK_CORE_WARN("        Max Circles per Batch           : {0}", CircleData::MaxCircles);
    IK_CORE_WARN("        Max Texture Slots Batch         : {0}", MaxTextureSlotsInShader);
    IK_CORE_WARN("        Memory Reserved for Vertex Data : {0} B ({1} KB) ", CircleData::MaxVertex * sizeof(CircleData::Vertex),  CircleData::MaxVertex * sizeof(CircleData::Vertex) / 1000.0f );

    if (s_QuadData)
        delete s_QuadData;

    if (s_CircleData)
        delete s_CircleData;
}

/// Initialize Quad Data
void BatchRenderer::InitQuadData() {
    PROFILE();
    // Alloc memory for Quad Data
    s_QuadData = new QuadData();
    
    IK_CORE_INFO("    Initialising the Quad Renderer");
    IK_CORE_INFO("        Max Quads per Batch             : {0}", QuadData::MaxQuad);
    IK_CORE_INFO("        Max Texture Slots Batch         : {0}", MaxTextureSlotsInShader);
    IK_CORE_INFO("        Memory Reserved for Vertex Data : {0} B ({1} KB) ", QuadData::MaxVertex * sizeof(QuadData::Vertex),  QuadData::MaxVertex * sizeof(QuadData::Vertex) / 1000.0f );
    
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
    
    // Create Index Buffer in GPU for storing Indices
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
    
    // Setup the Quad Shader
    s_QuadData->Shader = Renderer::GetShader(AssetManager::GetCoreAsset("shaders/2D/BatchQuadShader.glsl"));
}

/// Initialize Circle Data
void BatchRenderer::InitCircleData() {
    PROFILE();
    // Alloc memory for Circle Data
    s_CircleData = new CircleData();
    
    IK_CORE_INFO("    Initialising the Circle Renderer");
    IK_CORE_INFO("        Max Circle per Batch            : {0}", CircleData::MaxCircles);
    IK_CORE_INFO("        Max Texture Slots Batch         : {0}", MaxTextureSlotsInShader);
    IK_CORE_INFO("        Memory Reserved for Vertex Data : {0} B ({1} KB) ", CircleData::MaxVertex * sizeof(CircleData::Vertex),  CircleData::MaxVertex * sizeof(CircleData::Vertex) / 1000.0f );
    
    // Allocating the memory for vertex Buffer Pointer
    s_CircleData->VertexBufferBase = new CircleData::Vertex[QuadData::MaxVertex];
    
    // Create Pipeline instance
    s_CircleData->Pipeline = Pipeline::Create();
    
    // Create vertes Buffer
    s_CircleData->VertexBuffer = VertexBuffer::Create(CircleData::MaxVertex * sizeof(CircleData::Vertex));
    s_CircleData->VertexBuffer->AddLayout({
        { "a_Position",     ShaderDataType::Float3 },
        { "a_Color",        ShaderDataType::Float4 },
        { "a_TexCoords",    ShaderDataType::Float2 },
        { "a_TexIndex",     ShaderDataType::Float },
        { "a_TilingFactor", ShaderDataType::Float },
        { "a_Thickness",    ShaderDataType::Float },
        { "a_Fade",         ShaderDataType::Float },
        { "a_ObjectID",     ShaderDataType::Int },
    });
    s_CircleData->Pipeline->AddVertexBuffer(s_CircleData->VertexBuffer);

    // Create Index Buffer
    uint32_t* quadIndices = new uint32_t[CircleData::MaxIndices];
    
    uint32_t offset = 0;
    for (size_t i = 0; i < CircleData::MaxIndices; i += CircleData::IndicesForSingleCircle) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;
        
        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;
        
        offset += 4;
    }
    
    // Create Index Buffer in GPU for storing Indices
    std::shared_ptr<IndexBuffer> ib = IndexBuffer::CreateWithCount(quadIndices, CircleData::MaxIndices);
    s_CircleData->Pipeline->SetIndexBuffer(ib);
    delete[] quadIndices;
    
    // Creating white texture for colorful quads witout any texture or sprite
    uint32_t whiteTextureData = 0xffffffff;
    s_CircleData->TextureSlots[0] = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));

    // Setting basic Vertex point of quad
    s_CircleData->VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_CircleData->VertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    s_CircleData->VertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    s_CircleData->VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    
    // Setup the Circle Shader
    s_CircleData->Shader = Renderer::GetShader(AssetManager::GetCoreAsset("shaders/2D/BatchCircleShader.glsl"));
}

/// Begin the Batch renderer Scene
/// @param viewProj View projection matrix of Camera
void BatchRenderer::BeginBatch(const glm::mat4& cameraViewProj) {
    s_QuadData->Shader->Bind();
    s_QuadData->Shader->SetUniformMat4("u_ViewProjection", cameraViewProj);
    s_QuadData->StartBatch();
    
    s_CircleData->Shader->Bind();
    s_CircleData->Shader->SetUniformMat4("u_ViewProjection", cameraViewProj);
    s_CircleData->StartBatch();
}

/// End the Batch Rendere Scene
void BatchRenderer::EndBatch() {
    Flush();
}

/// Flush the scene for single batch
void BatchRenderer::Flush() {
    if (s_QuadData->IndexCount) {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_QuadData->VertexBufferPtr - (uint8_t*)s_QuadData->VertexBufferBase);
        s_QuadData->VertexBuffer->SetData(s_QuadData->VertexBufferBase, dataSize);
        
        // Bind textures
        for (size_t i = 0; i < s_QuadData->TextureSlotIndex; i++)
            s_QuadData->TextureSlots[i]->Bind((uint32_t)i);
        
        // Render the Scene
        s_QuadData->Shader->Bind();
        Renderer::DrawIndexed(s_QuadData->Pipeline, s_QuadData->IndexCount);
    }
    
    if (s_CircleData->IndexCount) {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_CircleData->VertexBufferPtr - (uint8_t*)s_CircleData->VertexBufferBase);
        s_CircleData->VertexBuffer->SetData(s_CircleData->VertexBufferBase, dataSize);

        // Bind textures
        for (size_t i = 0; i < s_CircleData->TextureSlotIndex; i++)
            s_CircleData->TextureSlots[i]->Bind((uint32_t)i);
        
        // Render the Scene
        s_CircleData->Shader->Bind();
        Renderer::DrawIndexed(s_CircleData->Pipeline, s_CircleData->IndexCount);
    }
}

/// start Next Renderer Batch
void BatchRenderer::NextBatch() {
    EndBatch();
    s_QuadData->StartBatch();
    s_CircleData->StartBatch();
}

/// Draw Quad API with color
/// @param transform Transformation of Quad
/// @param color Clor of Quad
/// @param entID Pixel ID of Quad
void BatchRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t entID) {
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    DrawTextureQuad(transform, nullptr, textureCoords, 1.0f, color, entID);
}

/// Draw Quad API for Texture
/// @param transform Quad transform
/// @param texture Texture to be uploaded
/// @param tilingFactor tiling factor
/// @param tintColor color
/// @param entID Pixel ID of Quad
void BatchRenderer::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tintColor, float tilingFactor, int32_t entID) {
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    DrawTextureQuad(transform, texture, textureCoords, tilingFactor, tintColor, entID);
}

/// Draw Quad API with sprite subtexture
/// @param transform Transformation matrix of Quad
/// @param subTexture Subtexture component
/// @param entID entity ID of Quad
void BatchRenderer::DrawQuad(const glm::mat4& transform, const std::shared_ptr<SubTexture>& subTexture, int32_t entID) {
    const glm::vec2* textureCoords = subTexture->GetTexCoord();
    DrawTextureQuad(transform, subTexture->GetTexture(), textureCoords, 1.0f, glm::vec4(1.0f), entID);
}


/// Draw Circle API with color
/// @param transform Transformation of Circle
/// @param color Clor of Circle
/// @param entID Pixel ID of Quad
void BatchRenderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int32_t entID) {
    DrawTextureCircle(transform, nullptr, 1.0f, color, thickness, fade, entID);
}

/// Draw Circle API for Texture
/// @param transform Circle transform
/// @param texture Texture to be uploaded
/// @param tilingFactor tiling factor
/// @param tintColor color
/// @param entID Pixel ID of Quad
void BatchRenderer::DrawCircle(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tintColor, float tilingFactor, float thickness, float fade, int32_t entID) {
    DrawTextureCircle(transform, texture, tilingFactor, tintColor, thickness, fade, entID);
}

/// Common Quad Renderer
/// @param transform Quad transform
/// @param texture Texture to be uploaded
/// @param textureCoords Texture coordinates
/// @param tilingFactor tiling factor
/// @param tintColor color
/// @param entID Pixel ID of Quad
void BatchRenderer::DrawTextureQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec2* textureCoords, float tilingFactor, const glm::vec4& tintColor, int32_t entID) {
    // If number of indices increase in batch then start new batch
    if (s_QuadData->IndexCount >= QuadData::MaxIndices) {
        IK_CORE_WARN("    Starts the new batch as number of indices ({0}) increases in the previous batch", s_QuadData->IndexCount);
        NextBatch();
    }
    
    float textureIndex = 0.0f;
    if (texture) {
        // Find if texture is already loaded in current batch
        for (size_t i = 1; i < s_QuadData->TextureSlotIndex; i++) {
            if (s_QuadData->TextureSlots[i].get() == texture.get()) {
                // Found the current textue in the batch
                textureIndex = (float)i;
                break;
            }
        }
        
        // If current texture slot is not pre loaded then load the texture in proper slot
        if (textureIndex == 0.0f) {
            // If number of slots increases max then start new batch
            if (s_QuadData->TextureSlotIndex >= MaxTextureSlotsInShader) {
                IK_CORE_WARN("    Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_QuadData->TextureSlotIndex);
                NextBatch();
            }
            
            // Loading the current texture in the first free slot slot
            textureIndex = (float)s_QuadData->TextureSlotIndex;
            s_QuadData->TextureSlots[s_QuadData->TextureSlotIndex] = texture;
            s_QuadData->TextureSlotIndex++;
        }
    }
    
    for (size_t i = 0; i < QuadData::VertexForSingleQuad; i++) {
        s_QuadData->VertexBufferPtr->Position     = transform * s_QuadData->VertexPositions[i];
        s_QuadData->VertexBufferPtr->Color        = tintColor;
        s_QuadData->VertexBufferPtr->TexCoords    = textureCoords[i];
        s_QuadData->VertexBufferPtr->TexIndex     = textureIndex;
        s_QuadData->VertexBufferPtr->TilingFactor = tilingFactor;
        s_QuadData->VertexBufferPtr->ObjectID     = entID;
        s_QuadData->VertexBufferPtr++;
    }
    
    s_QuadData->IndexCount += QuadData::IndicesForSingleQuad;
    
    RendererStatistics::Get().IndexCount += QuadData::IndicesForSingleQuad;
    RendererStatistics::Get().VertexCount += QuadData::VertexForSingleQuad;
}

/// Render the Circle
/// @param transform Transform of crcle
/// @param color color of circle
/// @param thickness thickneess
/// @param fade fade
/// @param entID Pixel ID of Quad
void BatchRenderer::DrawTextureCircle(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, float tilingFactor, const glm::vec4& color, float thickness, float fade, int32_t entID)
{
    // If number of indices increase in batch then start new batch
    if (s_CircleData->IndexCount >= CircleData::MaxIndices) {
        IK_CORE_WARN("Starts the new batch as number of indices ({0}) increases in the previous batch", s_CircleData->IndexCount);
        NextBatch();
    }
    
    float textureIndex = 0.0f;
    if (texture) {
        // Find if texture is already loaded in current batch
        for (size_t i = 1; i < s_CircleData->TextureSlotIndex; i++) {
            if (s_CircleData->TextureSlots[i].get() == texture.get()) {
                // Found the current textue in the batch
                textureIndex = (float)i;
                break;
            }
        }
        
        // If current texture slot is not pre loaded then load the texture in proper slot
        if (textureIndex == 0.0f) {
            // If number of slots increases max then start new batch
            if (s_CircleData->TextureSlotIndex >= MaxTextureSlotsInShader) {
                IK_CORE_WARN("    Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_CircleData->TextureSlotIndex);
                NextBatch();
            }
            
            // Loading the current texture in the first free slot slot
            textureIndex = (float)s_CircleData->TextureSlotIndex;
            s_CircleData->TextureSlots[s_CircleData->TextureSlotIndex] = texture;
            s_CircleData->TextureSlotIndex++;
        }
    }
    
    for (size_t i = 0; i < CircleData::VertexForSingleCircle; i++) {
        s_CircleData->VertexBufferPtr->Position     = transform * s_CircleData->VertexPositions[i];
        s_CircleData->VertexBufferPtr->Color        = color;
        s_CircleData->VertexBufferPtr->TexCoords    = 2.0f * s_CircleData->VertexPositions[i];
        s_CircleData->VertexBufferPtr->TexIndex     = textureIndex;
        s_CircleData->VertexBufferPtr->TilingFactor = tilingFactor;
        s_CircleData->VertexBufferPtr->Thickness    = thickness;
        s_CircleData->VertexBufferPtr->Fade         = fade;
        s_CircleData->VertexBufferPtr->ObjectID     = entID;
        s_CircleData->VertexBufferPtr++;
    }
    
    s_CircleData->IndexCount += CircleData::IndicesForSingleCircle;
    
    RendererStatistics::Get().IndexCount += CircleData::IndicesForSingleCircle;
    RendererStatistics::Get().VertexCount += CircleData::VertexForSingleCircle;
}
