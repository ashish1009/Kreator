//
//  PropertyGrid.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "PropertyGrid.hpp"
#include "Renderer/Utils/Renderer.hpp"
#include "Renderer/Graphics/Texture.hpp"

using namespace iKan;

std::shared_ptr<Texture> PropertyGrid::s_SearchTexture;

/// Initialize the Property Grid
void PropertyGrid::Init() {
    s_SearchTexture = Renderer::GetTexture(AssetManager::GetCoreAsset("textures/icons/search.png"));
}

/// Initialize the Property Grid
void PropertyGrid::Shutdown() {
    s_SearchTexture.reset();
}

/// Drag slider Float
/// @param label string to be printed
/// @param value float reference that will be modified with drag Slider
/// @param checkboxFlag Flag to have checkbox (conditional slider)
/// @param delta sgtep to be slided
/// @param resetValue Value after pressing the reset button
/// @param columnWidth width of column 1
/// @param columnWidth2 width of column 2
bool PropertyGrid::Float1(const char* label, float& value, bool* checkboxFlag, float delta, float resetValue, float columnWidth1, float columnWidth2) {
    bool modified = false;
    ImGuiIO& io   = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];
    
    ImGui::PushID(label);
    
    ImGui::Columns(2);
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(0, columnWidth1);
    
    // this flag is to check we need to have slider float or not
    bool propFlag;
    if (checkboxFlag) {
        ImGui::Checkbox(label, checkboxFlag);
        propFlag = *checkboxFlag;
    }
    else {
        ImGui::Text(label);
        propFlag = true;
    }
    ImGui::PopItemWidth();
    
    if (propFlag) {
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize)) {
            value = resetValue;
            modified = true;
        }
        
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value, delta, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopStyleVar();
    }
    ImGui::PopItemWidth();
    
    ImGui::Columns(1);
    ImGui::PopID();
    
    return modified;
}

/// Drag slider Float 2
/// @param label is string to be printed
/// @param value is glm::vec2 (2 float reference) that will be modified with drag slider
/// @param checkboxFlag Flag to have checkbox (conditional slider)
/// @param delta Step of modificaiton
/// @param resetValue Value after pressing the reset button
/// @param columnWidth width of column
bool PropertyGrid::Float2(const char* label, glm::vec2& value, bool* checkboxFlag, float delta, float resetValue, float columnWidth) {
    bool modified   = false;
    ImGuiIO& io     = ImGui::GetIO();
    auto boldFont   = io.Fonts->Fonts[0];
    
    ImGui::PushID(label);
    
    ImGui::Columns(2);
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(0, columnWidth);
    
    // this flag is to check we need to have slider float or not
    bool propFlag;
    if (checkboxFlag) {
        ImGui::Checkbox(label, checkboxFlag);
        propFlag = *checkboxFlag;
    }
    else {
        ImGui::Text(label);
        propFlag = true;
    }
    ImGui::PopItemWidth();
    
    if (propFlag) {
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize)) {
            value.x = resetValue;
            modified = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value.x, delta, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize)) {
            value.y = resetValue;
            modified = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##Y", &value.y, delta, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();
    }
    
    ImGui::Columns(1);
    ImGui::PopID();
    
    return modified;
}

/// Drag slider Float 3
/// @param label is string to be printed
/// @param value is glm::vec2 (2 float reference) that will be modified with drag slider
/// @param checkboxFlag Flag to have checkbox (conditional slider)
/// @param delta Delta value of modification
/// @param resetValue Value after pressing the reset button
/// @param columnWidth width of column
bool PropertyGrid::Float3(const char* label, glm::vec3& value, bool* checkboxFlag, float delta, float resetValue, float columnWidth) {
    bool modified = false;
    ImGuiIO& io   = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];
    
    ImGui::PushID(label);
    
    ImGui::Columns(2);
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(0, columnWidth);
    
    // this flag is to check we need to have slider float or not
    bool propFlag;
    if (checkboxFlag) {
        ImGui::Checkbox(label, checkboxFlag);
        propFlag = *checkboxFlag;
    }
    else {
        ImGui::Text(label);
        propFlag = true;
    }
    ImGui::PopItemWidth();
    
    if (propFlag) {
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize)) {
            value.x = resetValue;
            modified = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value.x, delta, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize)) {
            value.y = resetValue;
            modified = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##Y", &value.y, delta, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize)) {
            value.z = resetValue;
            modified = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##Z", &value.z, delta, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
    }
    
    ImGui::Columns(1);
    ImGui::PopID();
    
    return modified;
}

/// Read and write the String without lable. Modify the value if Modifiable is true then we can modify the value
/// Hint will be printed to String path
/// @param value Lable of Tag
/// @param hint Hint string to pring in Writable space
/// @param modifiable flag to check is it const or not
/// @param multiple flag to check multiple lines needed in strimg
/// @param numLines if multiple line supported then number of rows
/// @param error For red color
bool PropertyGrid::String(std::string& value, const char* hint, bool modifiable, bool multiple, int numLines, bool error) {
    bool modified = false;
    ImGui::PushID("String No Lable");
    
    if (hint) {
        ImGui::SameLine();
        HelpMarker(hint);
    }
    
    ImGui::PushItemWidth(-1);
    
    // Copy the Name of entity to buffer that will be dumy text in property pannel
    char buffer[256];
    strcpy(buffer, value.c_str());
    
    std::string UIContextId = "##" + (std::string)"String With No Lable";
    
    // To make string Red in case error flag is true
    if (error)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    
    if (modifiable) {
        // Take input text from User in Property pannel. that will be name(Tag) of Selected Entity
        if (multiple) {
            if (ImGui::InputTextEx(UIContextId.c_str(), hint, buffer, IM_ARRAYSIZE(buffer), ImVec2(50.0f, numLines * 20.0f), ImGuiInputTextFlags_Multiline)) {
                value    = buffer;
                modified = true;
            }
        }
        else {
            if (ImGui::InputTextWithHint(UIContextId.c_str(), hint, buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                value    = buffer;
                modified = true;
            }
        }
    }
    else
        ImGui::InputText(UIContextId.c_str(), (char*)value.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
    
    // Pop red color if error is enabled
    if (error)
        ImGui::PopStyleColor();

    ImGui::PopItemWidth();
    ImGui::PopID();
    
    return modified;
}

/// Read and write the String. Modify the value if Modifiable is true then we can modify the value
/// Hint will be printed to String path
/// @param label Lable of Tag
/// @param value Modifyable string
/// @param columnWidth Width of each Column
/// @param column2Width Width of each Column 2
/// @param hint Hint string to pring in Writable space
/// @param modifiable flag to check is it const or not
/// @param multiple flag to check multiple lines needed in strimg
/// @param numLines if multiple line supported then number of rows
/// @param error For red color
bool PropertyGrid::String(const char* label, std::string& value, float columnWidth, float column2Width, const char* hint, bool modifiable, bool multiple, int32_t numLines, bool error) {
    bool modified = false;
    ImGui::PushID(label);
    
    ImGui::Columns(2);
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label);
    ImGui::PopItemWidth();
    
    if (hint) {
        ImGui::SameLine();
        HelpMarker(hint);
    }
    
    ImGui::NextColumn();
    
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(1, column2Width);
    
    // Copy the Name of entity to buffer that will be dumy text in property pannel
    char buffer[256];
    strcpy(buffer, value.c_str());
    
    std::string UIContextId = "##" + (std::string)label;
    
    // To make string Red in case error flag is true
    if (error)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    
    if (modifiable) {
        // Take input text from User in Property pannel. that will be name(Tag) of Selected Entity
        if (multiple) {
            if (ImGui::InputTextEx(UIContextId.c_str(), hint, buffer, IM_ARRAYSIZE(buffer), ImVec2(column2Width, numLines * 20.0f), ImGuiInputTextFlags_Multiline)) {
                value    = buffer;
                modified = true;
            }
        }
        else {
            if (ImGui::InputTextWithHint(UIContextId.c_str(), hint, buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                value    = buffer;
                modified = true;
            }
        }
    }
    else
        ImGui::InputText(UIContextId.c_str(), (char*)value.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
    
    // Pop red color if error is enabled
    if (error)
        ImGui::PopStyleColor();
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    ImGui::Columns(1);
    
    ImGui::PopID();
    
    return modified;
}

/// Read and write the String
/// @param label Lable of Tag
/// @param value Modifyable string
/// @param hint Hint string to pring in Writable space
/// @param columnWidth Width of each Column
bool PropertyGrid::String(const char* label, const std::string& value, const char* hint, float columnWidth) {
    bool modified = false;
    ImGui::PushID(label);
    
    ImGui::Columns(2);
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label);
    ImGui::PopItemWidth();
    
    if (hint) {
        ImGui::SameLine();
        HelpMarker(hint);
    }
    
    ImGui::NextColumn();
    
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(1, 300.0f);
    
    // Copy the Name of entity to buffer that will be dumy text in property pannel
    char buffer[256];
    strcpy(buffer, value.c_str());
    
    std::string UIContextId = "##" + (std::string)label;
    
    ImGui::InputText(UIContextId.c_str(), (char*)value.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    ImGui::Columns(1);
    
    ImGui::PopID();
    
    return modified;
}

/// CheckBox
/// @param label is string to be printed
/// @param value is bool that will be modified with check box
/// @param columnWidth width of column
/// @param columnWidth2 width of column 2
bool PropertyGrid::CheckBox(const char* label, bool& value, float columnWidth, float columnWidth2) {
    bool modified = false;
    
    ImGui::Columns(2);
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label);
    ImGui::PopItemWidth();
    
    ImGui::NextColumn();
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(1, columnWidth2);
    std::string UIContextId = "##" + (std::string)label;
    
    if (ImGui::Checkbox(UIContextId.c_str(), &value))
        modified = true;
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    ImGui::Columns(1);
    
    return modified;
}

/// ComboDrop: Selectable drop option created
/// @param label is string to be printed
/// @param options option of drop box
/// @param currentValue current selection value (index of option vector)
/// @param colWidth1 column width 1
/// @param colWidth2 column width 1
uint32_t PropertyGrid::ComboDrop(const char* label, const std::vector<std::string>& options, uint32_t currentValue, float colWidth1, float colWidth2) {
    uint32_t result = currentValue;
    
    ImGui::Columns(2);
    ImGui::Text(label);
    
    ImGui::NextColumn();
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(1, colWidth2);
    
    std::string UIContextId = "##" + (std::string)label;
    
    const char* currentType = options[(int32_t)currentValue].c_str();
    if (ImGui::BeginCombo(UIContextId.c_str(), currentType)) {
        for (int32_t i = 0; i < options.size(); i++) {
            bool bIsSelected = currentType == options[i];
            if (ImGui::Selectable(options[i].c_str(), bIsSelected)) {
                currentType = options[i].c_str();
                result = i;
            }
            
            if (bIsSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    
    ImGui::Columns(1);
    ImGui::Separator();
    
    return result;
}

/// Helper to display a little (?) mark which shows a tooltip when hovered. In your own code you may want to display an
/// actual icon if you are using a merged icon fonts (see docs/FONTS.txt)
/// @param desc Helper string
/// @param iconChar Icon to be printed as Help Marker (By default (?) will be renderes)
/// @param sameLine Check is it in same line
void PropertyGrid::HelpMarker(const char* desc, const char* iconChar, bool sameLine) {
    if (sameLine)
        ImGui::SameLine();
    
    ImGui::TextDisabled(iconChar);
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

/// Create Imgae in current Imgui window
/// @param textureID Texture id of Image to be rendered
/// @param size size of image need to be rendered inside Imgui window (this size will be visible as texture)
/// @param uv0 Left
/// @param uv1 Right
void PropertyGrid::Image(void* textureID, const glm::vec2& size, const glm::vec2& uv0, const glm::vec2& uv1) {
    ImGui::Image((void*)textureID, ImVec2{ size.x, size.y }, ImVec2{ uv0.x, uv0.y }, ImVec2{ uv1.x, uv1.y });
}

/// img Button
/// @param lableId image lable string
/// @param texId texture id
/// @param size size of image
/// @param bgColor background color
/// @param padding padding
bool PropertyGrid::ImageButton(const std::string& lableId, uint32_t texId, const glm::vec2& size, const glm::vec4& bgColor, int32_t padding) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    
    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
    
    bool result = false;
    ImTextureID myTexId = (ImTextureID)((size_t)texId);
    ImGui::PushID(lableId.c_str());
    result = ImGui::ImageButton(myTexId, { size.x, size.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), padding, { bgColor.r, bgColor.g, bgColor.b, bgColor.a });
    ImGui::PopID();
    
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);

    return result;
}

/// img Button
/// @param lableId lable ID
/// @param texId Texture ID
/// @param size size of textire
bool PropertyGrid::ImageButton(int32_t lableId, uint32_t texId, const glm::vec2& size) {
    bool result = false;
    ImTextureID myTexId = (ImTextureID)((size_t)texId);
    ImGui::PushID(lableId);
    result = ImGui::ImageButton(myTexId, { size.x, size.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    ImGui::PopID();
    
    return result;
}

/// Read and write the String. Modify the value if Modifiable is true then we can modify the value
/// Hint will be printed to String path
/// @param value search string
/// @param hint Hint string to pring in Writable space
bool PropertyGrid::Search(char* value, const char* hint) {
    bool modified = false;
    ImGui::PushID("Search");
    ImGui::PushItemWidth(-1);
    
    PropertyGrid::ImageButton("Search", s_SearchTexture->GetRendererID(), { 20.0f, 20.0f });
    ImGui::SameLine();
    
    // Copy the Name of entity to buffer that will be dumy text in property pannel
    char buffer[256];
    strcpy(buffer, value);
    
    std::string UIContextId = "##" + std::string("Search");
    
    if (ImGui::InputTextWithHint("", hint, buffer, IM_ARRAYSIZE(buffer))) {
        strcpy(value, buffer);
        modified = true;
    }
    
    ImGui::PopItemWidth();
    ImGui::PopID();
    
    return modified;
}

/// Color Edit
/// @param colorRef Color Edit value
void PropertyGrid::ColorEdit(glm::vec4& colorRef) {
    static ImVec4 color         = ImVec4(colorRef.r, colorRef.g, colorRef.b, colorRef.a);
    static ImVec4 refColorValue = color;
    
    static bool alphaPreview = true, alphaHalfPreview = true;
    ImGui::Checkbox("Alpha", &alphaPreview);  ImGui::SameLine(); ImGui::Checkbox("Half Alpha", &alphaHalfPreview);
    ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_PickerHueWheel | (alphaHalfPreview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alphaPreview ? ImGuiColorEditFlags_AlphaPreview : 0));
    if (alphaPreview || alphaHalfPreview) miscFlags |= ImGuiColorEditFlags_AlphaBar; else miscFlags |= ImGuiColorEditFlags_NoAlpha;
    
    static bool sidePreview = true, refColor = false;
    ImGui::Checkbox("Side Preview", &sidePreview);
    if (sidePreview) {
        ImGui::SameLine();
        ImGui::Checkbox("Ref Color", &refColor);
        if (refColor) {
            ImGui::SameLine();
            ImGui::ColorEdit4("##RefColor", &refColorValue.x, ImGuiColorEditFlags_NoInputs | miscFlags);
        }
    }
    if (!sidePreview)
        miscFlags |= ImGuiColorEditFlags_NoSidePreview;
    
    ImGui::ColorPicker4("Back Ground##4", (float*)&color, miscFlags, refColor ? &refColorValue.x : NULL);
    
    colorRef = { color.x, color.y, color.z, color.w };
}
