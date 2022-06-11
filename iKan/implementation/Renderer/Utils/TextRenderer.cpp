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

using namespace iKan;

/// Store the Data used to render the Text
struct TextData {
    /// Renderer Data storage
    std::shared_ptr<Pipeline> Pipeline;
    std::shared_ptr<VertexBuffer> VertexBuffer;
    std::shared_ptr<Shader> Shader;

    std::map<char, std::shared_ptr<CharTexture>> CharTextureMap;
};
static TextData* s_TextData;

/// Initialize the text renderer
void TextRenderer::Init() {
    s_TextData = new TextData();
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Initialising the Text Renderer");
    
    // Create Pipeline instance
    s_TextData->Pipeline = Pipeline::Create();

    // Create vertes Buffer
    s_TextData->VertexBuffer = VertexBuffer::Create(sizeof(float) * 6 * 4);
    s_TextData->VertexBuffer->AddLayout({
        { "vertex", ShaderDataType::Float4 }
    });
    s_TextData->Pipeline->AddVertexBuffer(s_TextData->VertexBuffer);
    
    // Init the Text shader
    s_TextData->Shader = Renderer::GetShader(AssetManager::GetCoreAsset("shaders/2D/TextShader.glsl"));
    
    // FreeType
    FT_Library ft;
    IK_CORE_ASSERT(!FT_Init_FreeType(&ft), "Not able to load Freetype");
    
    // find path to font
    std::string fontName = AssetManager::GetCoreAsset("fonts/opensans/OpenSans-Bold.ttf");
    IK_CORE_ASSERT(!fontName.empty(), "Not able to load Font");
    
    // load font as face
    FT_Face face;
    IK_CORE_ASSERT(!FT_New_Face(ft, fontName.c_str(), 0, &face), "Unavle to load tha font to freetype");
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
/// @param x X Position of Text
/// @param y y Position of Text
/// @param scale Size of text
/// @param color Color of text
void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {
    // activate corresponding render state
    s_TextData->Shader->Bind();
    s_TextData->Shader->SetUniformFloat3("textColor", color);

    s_TextData->Pipeline->Bind();

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        std::shared_ptr<CharTexture> ch = s_TextData->CharTextureMap[*c];

        float xpos = x + ch->GetBearing().x * scale;
        float ypos = y - (ch->GetSize().y - ch->GetBearing().y) * scale;

        float w = ch->GetSize().x * scale;
        float h = ch->GetSize().y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        
        ch->Bind();
        s_TextData->VertexBuffer->SetData(vertices, sizeof(vertices));

        // render quad
        Renderer::DrawArrays(s_TextData->Pipeline, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch->GetAdvance() >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
}
