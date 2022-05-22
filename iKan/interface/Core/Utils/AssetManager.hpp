//
//  AssetManager.hpp
//  iKan
//
//  Created by iKan on 22/05/22.
//

#pragma once

namespace iKan {
    
    class AssetManager {
    public:
        /// Set the Base Core Asset path. This path should be relative to .exce.
        /// @param clientAssetPath asset folder path for Core
        static void SetClientAssetPath(const std::string& clientAssetPath);
        /// Set the Base Client Asset path. This path should be relative to .exce.
        /// @param coreAssetPath asset folder path for Client
        static void SetCoreAssetPath(const std::string& coreAssetPath);
        
        /// Return the Core Asset Base path, relative to .exce
        static const std::string& GetCoreAssetBasePath();
        /// Return the Client Asset Base path, relative to .exce
        static const std::string& GetClientAssetBasePath();
        
        /// Return the entire Asset path relative to .exce/
        /// NOTE: Add the asset path to base path
        /// @param assetPath aasset path relative to asset folder
        static std::string GetCoreAsset(const std::string& assetPath);
        /// Return the entire Asset path relative to .exce/
        /// NOTE: Add the asset path to base path
        /// @param assetPath aasset path relative to asset folder
        static std::string GetClientAsset(const std::string& assetPath);
        
    private:
        static std::string m_ClientAssetPath;
        static std::string m_CoreAssetPath;
    };
    
}
