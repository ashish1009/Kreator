//
//  ContentBrowserPannel.cpp
//  iKan
//
//  Created by iKan on 05/06/22.
//

#include "ContentBrowserPannel.hpp"
#include "Editor/PropertyGrid.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Gui/ImguiAPI.hpp"

using namespace iKan;

static std::vector<std::filesystem::path> s_PinedPaths;

/// ContentBrowserPannel constructor
/// @param rootPath Root Path
ContentBrowserPannel::ContentBrowserPannel(const std::string& rootPath) : m_RootPath(rootPath), m_CurrentDir(rootPath) {
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Content Browser Pannnel ... ");
    IK_CORE_INFO("    Base Path : {0}", m_RootPath.string());
    IK_LOG_SEPARATOR();

    m_PathHierarchy.emplace_back(m_CurrentDir);
    s_PinedPaths.emplace_back("../../../../../../../iKan.");
    s_PinedPaths.emplace_back("../../../../../../../iKan./iKan/Github/Product/iKan/iKan/assets");
    s_PinedPaths.emplace_back("../../../../../../../iKan./iKan/Github/Product/iKan/Kreator/assets/scenes"); // Temp
    s_PinedPaths.emplace_back("../../../../../../../iKan./iKan/Github/Product/iKan/Kreator/assets/models"); // Temp
    s_PinedPaths.emplace_back("../../../../../../../iKan./iKan/Github/Product/iKan/Kreator/assets/texture"); // Temp
}

/// Destroy Content browser pannel
ContentBrowserPannel::~ContentBrowserPannel() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Creating Content Browser Pannnel ... ");
    IK_CORE_WARN("    Base Path : {0}", m_RootPath.string());
    IK_LOG_SEPARATOR();
}

/// ContentBrowserPannel imgui renderer
void ContentBrowserPannel::RenderImgui(bool* pIsOpen) {
    ImGui::Begin("Content Browser");
    ImGui::PushID("Content Browser");
    
    TitleIcon();
    
    // Side Menue should be called before MainArea
    SideMenu();
    ImGui::SameLine();
    
    MainArea();
    
    ImGui::PopID();
    ImGui::End();
}

/// Render the Main for COntent browser
void ContentBrowserPannel::MainArea() {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("MainArea", ImVec2(ImGui::GetWindowContentRegionWidth() * (0.995f - s_SideChildWidth), ImGui::GetWindowHeight() - s_WindowYOffset), true, ImGuiWindowFlags_HorizontalScrollbar);
    
    int32_t pushId = 0;
    static ImVec2 initPos = ImGui::GetCursorPos();
    for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDir)) {
        const auto& path            = directoryEntry.path();
        std::string filenameString  = path.filename().string();
        
        if (m_Filter.PassFilter(filenameString.c_str())) {
            std::shared_ptr<Texture> iconTexture;
            static bool isDirectory = false;
            if (directoryEntry.is_directory()) {
                iconTexture = m_TileIconTexture.FolderIcon;
                isDirectory = true;
            }
            else {
                if (".png" == path.extension())         iconTexture = m_TileIconTexture.PngIcon;
                else if (".jpg" == path.extension())    iconTexture = m_TileIconTexture.JpgIcon;
                else if (".cpp" == path.extension())    iconTexture = m_TileIconTexture.Cpp;
                else if (".h" == path.extension())      iconTexture = m_TileIconTexture.H;
                else if (".c" == path.extension())      iconTexture = m_TileIconTexture.C;
                else                                    iconTexture = m_TileIconTexture.FileIcon;
                
                isDirectory = false;
            }
            
            static float iconSizeHeight = 64.0f;
            static float iconSizeWidth  = 50.0f;
                        
            ImGui::PushID(filenameString.c_str());
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + ((iconSizeWidth + 30.0f) * pushId), initPos.y));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            
            bool pressed = PropertyGrid::ImageButton(pushId, iconTexture->GetRendererID(), { iconSizeWidth, iconSizeHeight });
            
            if (pressed) {
                if (isDirectory) {
                    m_PrevDir.emplace_back(m_CurrentDir);
                    m_CurrentDir /= path.filename();
                    m_PathHierarchy.emplace_back(m_CurrentDir);
                    m_ForwardDir.clear();
                }
            }
            
            // Drag Begin here
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                std::string path = directoryEntry.path().string();
                const char* ch = path.c_str();
                ImGui::SetDragDropPayload("SelectedFile", (void*)ch, path.size(), ImGuiCond_Always);
                PropertyGrid::ImageButton(pushId, iconTexture->GetRendererID(), { 12.0f, 16.0f });
                ImGui::EndDragDropSource();
            }
            
            static float wrapWidth = 70.0f;
            ImGui::PushID("CBP_Main_Area_FileNameButton");
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + ((iconSizeWidth + 30.0f) * pushId), initPos.y + iconSizeHeight + 10.0f));
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrapWidth);
            ImGui::Button(filenameString.c_str(), ImVec2(iconSizeWidth + 5, 20));
            ImGui::PopTextWrapPos();
            ImGui::PopStyleColor();
            ImGui::PopID();

            pushId++;
            ImGui::PopID();
        } // Pass Filter
    } // Iteration
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

/// Render the Main for COntent browser
void ContentBrowserPannel::SideMenu() {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("SideArea", ImVec2(ImGui::GetWindowContentRegionWidth() * s_SideChildWidth, ImGui::GetWindowHeight() - s_WindowYOffset), true, ImGuiWindowFlags_HorizontalScrollbar);
    
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
    
    bool opened = ImGui::TreeNodeEx((void*)"##SizeMenu", flags, "Contents");
    if (opened) {
        for (auto pinnedPath : s_PinedPaths) {
            auto filename = pinnedPath.filename().c_str();
            bool opened = ImGui::TreeNodeEx(filename, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ((pinnedPath == m_CurrentDir) ? ImGuiTreeNodeFlags_Selected : 0), filename);
            if (ImGui::IsItemClicked()) {
                if (pinnedPath != m_CurrentDir) {
                    m_PrevDir.clear();
                    m_PathHierarchy.clear();
                    m_CurrentDir = pinnedPath;
                    m_PathHierarchy.emplace_back(m_CurrentDir);
                }
            }
            if (opened) {
                for (auto& directoryEntry : std::filesystem::directory_iterator(pinnedPath)) {
                    const auto& path            = directoryEntry.path();
                    auto relativePath           = std::filesystem::relative(path, m_RootPath);
                    std::string filenameString  = relativePath.filename().string();
                    
                    if (ImGui::TreeNodeEx(filenameString.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth, filenameString.c_str()))
                        ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        
        ImGui::TreePop();
    }
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

/// Render the Title Icons for COntent browser
void ContentBrowserPannel::TitleIcon() {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Title Area", ImVec2(ImGui::GetWindowContentRegionWidth(), 35.0f), true, ImGuiWindowFlags_NoScrollbar);
    
    ImGui::Columns(3);
    ImGui::SetColumnWidth(0, 100);
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, (35.0f - 20.0f) / 2));
    Back();             ImGui::SameLine();
    Forward();          ImGui::SameLine();
    Home();             ImGui::SameLine();
    
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, 200);
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, (35.0f - 20.0f) / 2));
    Search();
    
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, 200);
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, (35.0f - 20.0f) / 2));
    PathHistory();
    ImGui::NextColumn();
    ImGui::Columns(1);
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

/// Back Button
void ContentBrowserPannel::Back() {
    if (PropertyGrid::ImageButton("Back", m_TileIconTexture.Back->GetRendererID(), { 20.0f, 20.0f })) {
        if (m_PrevDir.empty())
            return;
        
        if (m_CurrentDir != std::filesystem::path(m_RootPath)) {
            if (m_PrevDir.back() == m_CurrentDir.parent_path())
                m_PathHierarchy.pop_back();

            else {
                for (auto prevIt = m_PrevDir.begin(); prevIt != m_PrevDir.end(); prevIt++) {
                    if (*prevIt == m_CurrentDir) {
                        m_PathHierarchy.insert(m_PathHierarchy.end(), ++prevIt, m_PrevDir.end());
                        break;
                    }
                }
            }
            m_ForwardDir.emplace_back(m_CurrentDir);
            m_CurrentDir = m_PrevDir.back();
            m_PrevDir.pop_back();
        }
    }
}

/// Forward Button
void ContentBrowserPannel::Forward() {
    if (PropertyGrid::ImageButton("Forward", m_TileIconTexture.Forward->GetRendererID(), { 20.0f, 20.0f })) {
        if (m_ForwardDir.empty())
            return;
        
        m_PrevDir.emplace_back(m_CurrentDir);
        m_CurrentDir = m_ForwardDir.back();
        m_ForwardDir.pop_back();
        
        bool found = false;
        size_t pos = 0;
        for (auto path : m_PathHierarchy) {
            pos++;
            if (path == m_CurrentDir) {
                found = true;
                break;
            }
        }
        if (found)
            m_PathHierarchy.erase(m_PathHierarchy.begin() + pos, m_PathHierarchy.end());
        else
            m_PathHierarchy.emplace_back(m_CurrentDir);
    }
}

/// Home Button
void ContentBrowserPannel::Home() {
    if (PropertyGrid::ImageButton("Home", m_TileIconTexture.Home->GetRendererID(), { 20.0f, 20.0f })) {
        m_PrevDir.clear();
        m_CurrentDir = m_RootPath;
        m_PathHierarchy.clear();
        m_PathHierarchy.emplace_back(m_CurrentDir);
    }
}

/// Path History Buttons
void ContentBrowserPannel::PathHistory() {
    size_t i = 0;
    for (auto path : m_PathHierarchy) {
        i++;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        if (ImGui::Button(path.filename().c_str())) {
            if (path != m_CurrentDir)
                m_PrevDir.emplace_back(m_CurrentDir);
            
            m_CurrentDir = path;
            m_PathHierarchy.erase(m_PathHierarchy.begin() + i, m_PathHierarchy.end());
        }
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::Text("|");
    }
}

/// Search Tab
void ContentBrowserPannel::Search() {
    // Search filter
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 16);
    if (PropertyGrid::Search(m_Filter.InputBuf, "Search ... "))
        m_Filter.Build();
    
    ImGui::SameLine();
    PropertyGrid::ImageButton("Search", m_TileIconTexture.Search->GetRendererID(), { 20.0f, 20.0f });
}

