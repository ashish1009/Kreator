//
//  TextRendering.cpp
//  iKan
//
//  Created by iKan on 11/06/22.
//

#include "TextRenderer.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Renderer/Utils/Renderer.hpp"
#include "Renderer/Utils/RendererStats.hpp"

using namespace iKan;

/// Store the Data used to render the Text
struct TextData {
    struct Vertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        
        int32_t ObjectID; // Pixel ID of Quad
    };
    
    // --------------- Constants -----------------
    // Fixed Constants
    static constexpr uint32_t VertexForSingleChar = 6;
    
    /// Renderer Data storage
    std::shared_ptr<Pipeline> Pipeline;
    std::shared_ptr<VertexBuffer> VertexBuffer;
    std::shared_ptr<Shader> Shader;

    /// Map to store the Char Texture for each character
    std::map<char, std::shared_ptr<CharTexture>> CharTextureMap;

    // -------------- Variables ------------------
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* VertexBufferBase = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* VertexBufferPtr = nullptr;

    /// Store the Base Texture coordinga
    glm::vec2 BaseTexCoords[VertexForSingleChar];
        
    /// Constructor
    TextData() {
        IK_CORE_INFO("Creating Text Data instance ...");
    }
    /// Destructir
    virtual ~TextData() {
        IK_CORE_WARN("Destroying Text Data instance and clearing the data !!!");
        delete [] VertexBufferBase;
        VertexBufferBase = nullptr;

        RendererStatistics::Get().VertexBufferSize -= TextData::VertexForSingleChar * sizeof(TextData::Vertex);
    }
};
static TextData* s_TextData;

/// Initialize the text renderer
void TextRenderer::Init() {
    s_TextData = new TextData();
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Initialising the Text Renderer");
    IK_CORE_INFO("    Memory Reserved for Vertex Data : {0} B ({1} KB) ", TextData::VertexForSingleChar * sizeof(TextData::Vertex),  TextData::VertexForSingleChar * sizeof(TextData::Vertex) / 1000.0f );

    // Allocating the memory for vertex Buffer Pointer
    s_TextData->VertexBufferBase = new TextData::Vertex[TextData::VertexForSingleChar];

    // Create Pipeline instance
    s_TextData->Pipeline = Pipeline::Create();

    // Create vertes Buffer
    s_TextData->VertexBuffer = VertexBuffer::Create(sizeof(TextData::Vertex) * TextData::VertexForSingleChar);
    s_TextData->VertexBuffer->AddLayout({
        { "a_Position",  ShaderDataType::Float3 },
        { "a_Color",     ShaderDataType::Float4 },
        { "a_TexCoords", ShaderDataType::Float2 },
        { "a_ObjectID",  ShaderDataType::Int },
    });
    s_TextData->Pipeline->AddVertexBuffer(s_TextData->VertexBuffer);
    
    // Init the Text shader
    s_TextData->Shader = Renderer::GetShader(AssetManager::GetCoreAsset("shaders/2D/TextShader.glsl"));
    
    // Base Texture coordinate for Char rendering
    s_TextData->BaseTexCoords[0] = { 0.0f, 0.0f };
    s_TextData->BaseTexCoords[1] = { 0.0f, 1.0f };
    s_TextData->BaseTexCoords[2] = { 1.0f, 1.0f };
    s_TextData->BaseTexCoords[3] = { 0.0f, 0.0f };
    s_TextData->BaseTexCoords[4] = { 1.0f, 1.0f };
    s_TextData->BaseTexCoords[5] = { 1.0f, 0.0f };
}

/// Load the font for freetype
/// @param fontPath path of font
void TextRenderer::LoadFreetype(const std::string& fontPath) {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Loading Freetype Font for Text Data");
    IK_CORE_INFO("    Path : {0}", fontPath.c_str());

    FT_Library ft;
    IK_CORE_ASSERT(!FT_Init_FreeType(&ft), "Not able to load Freetype");
    
    // find path to font
    IK_CORE_ASSERT(!fontPath.empty(), "Not able to load Font");
    
    // load font as face
    FT_Face face;
    IK_CORE_ASSERT(!FT_New_Face(ft, fontPath.c_str(), 0, &face), "Unavle to load tha font to freetype");
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            IK_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // now store character for later use
        std::shared_ptr<CharTexture> charTexture = CharTexture::Create(face,
                                                                       glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                                                       glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                                                       static_cast<uint32_t>(face->glyph->advance.x));
        s_TextData->CharTextureMap.insert(std::pair<char, std::shared_ptr<CharTexture>>(c, charTexture));
    }

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

/// Shut down the text renderer
void TextRenderer::Shutdown() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Shutting down the Text Renderer !!!");
    IK_CORE_INFO("    Memory Reserved for Vertex Data : {0} B ({1} KB) ", TextData::VertexForSingleChar * sizeof(TextData::Vertex),  TextData::VertexForSingleChar * sizeof(TextData::Vertex) / 1000.0f );
    if (s_TextData)
        delete s_TextData;
}

/// Begin the Batch renderer Scene
/// @param viewProj View projection matrix of Camera
void TextRenderer::BeginBatch(const glm::mat4& cameraViewProj) {
    s_TextData->Shader->Bind();
    s_TextData->Shader->SetUniformMat4("u_ViewProjection", cameraViewProj);
}

/// Render the Text in Window
/// @param text Text to be rendereed
/// @param transform Transform of COlor
/// @param color Color of text
/// @param entID Pixel ID of Text
void TextRenderer::RenderText(std::string text, const glm::mat4& transform, const glm::vec4& color, int32_t entID) {
    // TODO: Rotation of Char is not suppirted Yet
    // iterate through all characters
    std::string::const_iterator c;
    glm::vec3 position, rotation, scale;
    Math::DecomposeTransform(transform, position, rotation, scale);
    
    // Rescaling the text as it render too large in begining
    scale *= 0.01;
    
    for (c = text.begin(); c != text.end(); c++) {
        std::shared_ptr<CharTexture> ch = s_TextData->CharTextureMap[*c];

        float xpos = position.x + ch->GetBearing().x * scale.x;
        float ypos = position.y - (ch->GetSize().y - ch->GetBearing().y) * scale.y;
        float zpos = position.z;

        float w = ch->GetSize().x * scale.x;
        float h = ch->GetSize().y * scale.y;
        
        // update VBO for each character
        glm::vec3 vertexPosition[TextData::VertexForSingleChar] = {
            { xpos,     ypos + h, zpos },
            { xpos,     ypos    , zpos },
            { xpos + w, ypos    , zpos },

            { xpos,     ypos + h, zpos },
            { xpos + w, ypos    , zpos },
            { xpos + w, ypos + h, zpos },
        };
        
        s_TextData->VertexBufferPtr = s_TextData->VertexBufferBase;
        
        // Each Vertex of Char
        for (size_t i = 0; i < TextData::VertexForSingleChar; i++) {
            s_TextData->VertexBufferPtr->Position     = vertexPosition[i];
            s_TextData->VertexBufferPtr->Color        = color;
            s_TextData->VertexBufferPtr->TexCoord     = s_TextData->BaseTexCoords[i];
            s_TextData->VertexBufferPtr->ObjectID     = entID;
            s_TextData->VertexBufferPtr++;
        }
        
        uint32_t dataSize = (uint32_t)((uint8_t*)s_TextData->VertexBufferPtr - (uint8_t*)s_TextData->VertexBufferBase);
        s_TextData->VertexBuffer->SetData(s_TextData->VertexBufferBase, dataSize);
        
        // Render the Scene
        s_TextData->Shader->Bind();
        ch->Bind();
        Renderer::DrawArrays(s_TextData->Pipeline, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        position.x += (ch->GetAdvance() >> 6) * scale.x; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    
    RendererStatistics::Get().VertexCount += TextData::VertexForSingleChar;
}

/// Render the Text in Window
/// @param text Text to be rendereed
/// @param position Text Poistion
/// @param scale Text Poistion
/// @param color Color of text
void TextRenderer::RenderText(std::string text, glm::vec3 position, const glm::vec2& scale, const glm::vec4& color) {
    // iterate through all characters
    std::string::const_iterator c;
    
    for (c = text.begin(); c != text.end(); c++) {
        std::shared_ptr<CharTexture> ch = s_TextData->CharTextureMap[*c];

        float xpos = position.x + ch->GetBearing().x * scale.x;
        float ypos = position.y - (ch->GetSize().y - ch->GetBearing().y) * scale.y;
        float zpos = position.z;

        float w = ch->GetSize().x * scale.x;
        float h = ch->GetSize().y * scale.y;
        
        // update VBO for each character
        glm::vec3 vertexPosition[TextData::VertexForSingleChar] = {
            { xpos,     ypos + h, zpos },
            { xpos,     ypos    , zpos },
            { xpos + w, ypos    , zpos },

            { xpos,     ypos + h, zpos },
            { xpos + w, ypos    , zpos },
            { xpos + w, ypos + h, zpos },
        };
        
        s_TextData->VertexBufferPtr = s_TextData->VertexBufferBase;
        
        // Each Vertex of Char
        for (size_t i = 0; i < TextData::VertexForSingleChar; i++) {
            s_TextData->VertexBufferPtr->Position     = vertexPosition[i];
            s_TextData->VertexBufferPtr->Color        = color;
            s_TextData->VertexBufferPtr->TexCoord     = s_TextData->BaseTexCoords[i];
            s_TextData->VertexBufferPtr->ObjectID     = -1;
            s_TextData->VertexBufferPtr++;
        }
        
        uint32_t dataSize = (uint32_t)((uint8_t*)s_TextData->VertexBufferPtr - (uint8_t*)s_TextData->VertexBufferBase);
        s_TextData->VertexBuffer->SetData(s_TextData->VertexBufferBase, dataSize);
        
        // Render the Scene
        s_TextData->Shader->Bind();
        ch->Bind();
        Renderer::DrawArrays(s_TextData->Pipeline, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        position.x += (ch->GetAdvance() >> 6) * scale.x; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    
    RendererStatistics::Get().VertexCount += TextData::VertexForSingleChar;

}
