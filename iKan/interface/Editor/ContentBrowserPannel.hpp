//
//  ContentBrowserPannel.hpp
//  iKan
//
//  Created by iKan on 05/06/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {

    /// Content browser Pannel class
    class ContentBrowserPannel {
    public:
        /// Content Browser Pannel Constructor
        /// @param rootPath root path of Content browser pannel
        ContentBrowserPannel(const std::string& rootPath);
        /// Destructor of Content browser pannel
        ~ContentBrowserPannel();
        
        /// Render Imgui for Contenct browser pannel
        void RenderImgui(bool* pIsOpen = nullptr);
        /// return the Current directory
        const std::filesystem::path& GetCurrentDir() const { return m_CurrentDir; }
        /// return the Root directory
        const std::filesystem::path& GetRootDir() const { return m_RootPath; }
        
    private:
        // Functions
        void TitleIcon();
        void MainArea();
        void SideMenu();
        
        void Back();
        void Forward();
        void Home();
        void PathHistory();
        void Search();
        
        // Variables
        static constexpr float s_SideChildWidth = 0.15;
        static constexpr float s_WindowYOffset  = 55.0f;
        
        std::filesystem::path m_CurrentDir;
        std::filesystem::path m_RootPath;
        
        std::vector<std::filesystem::path> m_PrevDir;
        std::vector<std::filesystem::path> m_ForwardDir;
        std::vector<std::filesystem::path> m_PathHierarchy;
        
        ImGuiTextFilter m_Filter;
        
        // TODO: Move some other place.
        struct TileIconTexture {
            const std::shared_ptr<Texture> FolderIcon = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/folder.png"));
            const std::shared_ptr<Texture> FileIcon   = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/file.png"));
            const std::shared_ptr<Texture> JpgIcon    = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/jpg.png"));
            const std::shared_ptr<Texture> PngIcon    = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/png.png"));
            
            const std::shared_ptr<Texture> Obj        = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/obj.png"));
            const std::shared_ptr<Texture> Fbx        = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/fbx.png"));

            const std::shared_ptr<Texture> Font       = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/font.png"));

            const std::shared_ptr<Texture> Cpp        = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/cpp.png"));
            const std::shared_ptr<Texture> C          = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/c.png"));
            const std::shared_ptr<Texture> H          = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/h.png"));

            const std::shared_ptr<Texture> Back       = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/back.png"));
            const std::shared_ptr<Texture> Forward    = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/forward.png"));

            const std::shared_ptr<Texture> Home       = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/contentBrowser/home.png"));
            
            const std::shared_ptr<Texture> Search     = Renderer::GetTexture(AssetManager::GetCoreAsset("/textures/icons/search.png"));
        };
        TileIconTexture m_TileIconTexture;

    };
    
}
