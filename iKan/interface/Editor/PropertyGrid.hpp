//
//  PropertyGrid.hpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

#include <imgui_internal.h>

namespace iKan {
    
    class Texture;
    
    /// Store the Wrapper API for showing Data in Imgui window
    class PropertyGrid {
    public:
        /// Initialize the Proeprty Grid.
        static void Init();
        
        /// Drag Float value
        /// @param label Lable for Variable
        /// @param value Value reference to be changed
        /// @param checkboxFlag flag to render either checkbox or lable
        /// @param delta step of Drag in float
        /// @param resetValue value of reset button pressed
        /// @param columnWidth width of column of lable
        /// @param columnWidth2 width of column of Value
        static bool Float1(const char* label, float& value, bool* checkboxFlag = nullptr, float delta = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f, float columnWidth2 = 100.0f);
        /// Drag Float2 (glm::vec2) value
        /// @param label Lable for Variable
        /// @param value Value reference to be changed
        /// @param checkboxFlag flag to render either checkbox or lable
        /// @param delta step of Drag in float
        /// @param resetValue value of reset button pressed
        /// @param columnWidth width of column of lable
        static bool Float2(const char* label, glm::vec2& value, bool* checkboxFlag = nullptr, float delta = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f);
        /// Drag Float3 (glm::vec3) value
        /// @param label Lable for Variable
        /// @param value Value reference to be changed
        /// @param checkboxFlag flag to render either checkbox or lable
        /// @param delta step of Drag in float
        /// @param resetValue value of reset button pressed
        /// @param columnWidth width of column of lable
        static bool Float3(const char* label, glm::vec3& value, bool* checkboxFlag = nullptr, float delta = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f );
        
        /// Render the Text box to enter string
        /// @param value Value need to be edit or entered in box
        /// @param hint Hint to be shown in the text box. Null of no hind is given
        /// @param modifiable flag to check is string is constant or modifiable
        /// @param multople flag to check if there are multiple line in text box
        /// @param numLines if multipleLine is true then check number of line are there in text box
        /// @param error flag to check if text is error (if true text printed as red)
        static bool String(std::string& value, const char* hint = nullptr, bool modifiable = true, bool multiple = false, int numLines = 1, bool error = false);
        /// Render the Text box to enter string
        /// @param label Lable for Variable
        /// @param value Value need to be edit or entered in box
        /// @param columnWidth width of column of lable
        /// @param columnWidth2 width of column of Value
        /// @param hint Hint to be shown in the text box. Null of no hind is given
        /// @param modifiable flag to check is string is constant or modifiable
        /// @param multople flag to check if there are multiple line in text box
        /// @param numLines if multipleLine is true then check number of line are there in text box
        /// @param error flag to check if text is error (if true text printed as red)
        static bool String(const char* label, std::string& value, float columnWidth, float column2Width = 300.0f, const char* hint = nullptr, bool modifiable = true, bool multiple = false, int32_t numLines = 1, bool error = false);
        /// Render the Text box to enter string
        /// @param label Lable for Variable
        /// @param value Value need to be edit or entered in box
        /// @param hint Hint to be shown in the text box. Null of no hind is given
        /// @param columnWidth width of column of lable
        static bool String(const char* label, const std::string& value, const char* hint, float columnWidth);
        
        /// Render Check box
        /// @param label Lable of checkbox
        /// @param value value to be updated
        /// @param columnWidth width of column of lable
        /// @param columnWidth2 width of column of Value
        static bool CheckBox(const char* label, bool& value, float columnWidth = 100.0f, float columnWidth2 = 300.0f);
        
        /// ComboDrop: Selectable drop option created
        /// @param label is string to be printed
        /// @param options option of drop box
        /// @param currentValue current selection value (index of option vector)
        /// @param colWidth1 column width 1
        /// @param colWidth2 column width 1
        static uint32_t ComboDrop(const char* label, const std::vector<std::string>& options, uint32_t currentValue, float colWidth1 = 100.0f, float colWidth2 = 200.0f);

        /// Render a hint icon (?) and on hovered A hint string will be shown
        /// @param desc String to be used as hint
        /// @param iconChar Icon to be printed as Help Marker (By default (?) will be renderes)
        /// @param sameLine check for same line icon or in new line
        static void HelpMarker(const char* desc, const char* iconChar = "?", bool sameLine = true);
        
        /// Create Imgae in current Imgui window
        /// @param textureID Texture id of Image to be rendered
        /// @param size size of image need to be rendered inside Imgui window (this size will be visible as texture)
        /// @param uv0 Left
        /// @param uv1 Right
        static void Image(void* textureID, const glm::vec2& size, const glm::vec2& uv0, const glm::vec2& uv1);
        
        /// img Button
        /// @param lableId image lable string
        /// @param texId texture id
        /// @param size size of image
        /// @param bgColor background color
        /// @param padding padding
        static bool ImageButton(const std::string& lableId, uint32_t texId, const glm::vec2& size, const glm::vec4& bgColor = glm::vec4(0.0f), int32_t padding = 0);

        /// img Button
        /// @param lableId lable ID
        /// @param texId Texture ID
        /// @param size size of textire
        static bool ImageButton(const int32_t lableId, uint32_t texId, const glm::vec2& size);
        
        /// Read and write the String. Modify the value if Modifiable is true then we can modify the value
        /// Hint will be printed to String path
        /// @param value search string
        /// @param hint Hint string to pring in Writable space
        static bool Search(char* value, const char* hint);
        
        /// Drop content from content prowser pannel and call the function
        /// @param uiFunction Function
        template<typename UIFunction> static void DropConent(UIFunction uiFunction) {
            if (ImGui::BeginDragDropTarget() && !ImGui::IsMouseDragging(0) && ImGui::IsMouseReleased(0)) {
                if (const ImGuiPayload* data = ImGui::AcceptDragDropPayload("SelectedFile", ImGuiDragDropFlags_AcceptBeforeDelivery)) {
                    char* filePath = new char[data->DataSize + 1];
                    memcpy(filePath, (char*)data->Data, data->DataSize);
                    filePath[data->DataSize] = '\0';
                    
                    uiFunction(filePath);
                    delete[] filePath;
                }
                ImGui::EndDragDropTarget();
            }
        }

    private:
        PropertyGrid() = default;
        MAKE_SINGLETON(PropertyGrid);

        static std::shared_ptr<Texture> s_SearchTexture;
    };
    
}
