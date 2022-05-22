//
//  AssetManager.cpp
//  iKan
//
//  Created by iKan on 22/05/22.
//

#include "AssetManager.hpp"

using namespace iKan;

std::string AssetManager::m_ClientAssetPath;
std::string AssetManager::m_CoreAssetPath;

const std::string& AssetManager::GetCoreAssetBasePath() { return m_CoreAssetPath; }
const std::string& AssetManager::GetClientAssetBasePath() { return m_ClientAssetPath; }

std::string AssetManager::GetCoreAsset(const std::string& assetPath) { return m_CoreAssetPath + assetPath; }
std::string AssetManager::GetClientAsset(const std::string& assetPath) { return m_ClientAssetPath + assetPath; }

void AssetManager::SetCoreAssetPath(const std::string &coreAssetPath) {
    m_CoreAssetPath = coreAssetPath;
    IK_CORE_INFO("Core Asset path is set to {0}", m_CoreAssetPath);
}

void AssetManager::SetClientAssetPath(const std::string &clientAssetPath) {
    m_ClientAssetPath = clientAssetPath;
    IK_CORE_INFO("Client Asset path is set to {0}", m_ClientAssetPath);
}
