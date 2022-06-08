//
//  Mesh.cpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#include "Mesh.hpp"
#include "Renderer/Utils/Renderer.hpp"
#include "Renderer/Utils/RendererStats.hpp"

// Assimp Files
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace iKan;

static constexpr aiTextureType s_AiMaterialType[MaxPBRTextureSupported] = { aiTextureType_DIFFUSE, aiTextureType_NORMALS, aiTextureType_SHININESS, aiTextureType_METALNESS, aiTextureType_SPECULAR };
static const std::string s_Texname[MaxPBRTextureSupported] = { "Albedo", "Normal", "Roughness", "Metallic", "Specular" };
static const std::string s_ShaderTextureName[MaxPBRTextureSupported] = {
    "u_" + s_Texname[0] + "Texture",
    "u_" + s_Texname[1] + "Texture",
    "u_" + s_Texname[2] + "Texture",
    "u_" + s_Texname[3] + "Texture",
    "u_" + s_Texname[4] + "Texture"
};
static const uint32_t s_MeshImportFlags =
                            aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
                            aiProcess_Triangulate |             // Make sure we're triangles
                            aiProcess_SortByPType |             // Split meshes by primitive type
                            aiProcess_GenNormals |              // Make sure we have legit normals
                            aiProcess_GenUVCoords |             // Convert UVs if required
                            aiProcess_OptimizeMeshes |          // Batch draws where possible
                            aiProcess_ValidateDataStructure;    // Validation

namespace MeshUtils {
    
    /// return glm mat4 from aiMatrix
    /// @param matrix ai Matrix
    static glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix) {
        glm::mat4 result;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
        result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
        result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
        result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
        return result;
    }

}

/// Add bone to animated Verttex
/// @param BoneID bone ID
/// @param Weight bone weight
void AnimatedVertex::AddBoneData(uint32_t BoneID, float Weight) {
    for (size_t i = 0; i < 4; i++) {
        if (Weights[i] == 0.0) {
            IDs[i] = BoneID;
            Weights[i] = Weight;
            return;
        }
    }
    IK_CORE_WARN("    Vertex has more than four bones/weights affecting it, extra data will be discarded (BoneID : {0}, Weight : {1})", BoneID, Weight);
}

/// Create mesh API
/// @param path Mesh File path
/// @param entityId Entity ID of Mesh
std::shared_ptr<Mesh> Mesh::Create(const std::string& path, uint32_t entityId, bool loadPredefinedMaterial) {
    return std::make_shared<Mesh>(path, entityId, loadPredefinedMaterial);
}

/// Mesh Constructor
/// @param path File Path
Mesh::Mesh(const std::string& path, uint32_t entityId, bool loadPredefinedMaterial)
: m_Path(path), m_Directory(StringUtils::GetDirectoryFromFilePath(path)), m_Name(StringUtils::GetNameFromFilePath(path)), m_EntityId(entityId) {
    PROFILE();
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Mesh ...");
    IK_CORE_INFO("    File Path : {0}", m_Path);
    IK_CORE_INFO("    Mesh Name : {0}", m_Name);
    IK_CORE_INFO("    Entity ID : {0}", m_EntityId);
    
    LoadMesh(loadPredefinedMaterial);
    
    IK_LOG_SEPARATOR();
}

/// Mesh Destructor
Mesh::~Mesh() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Mesh ...");
    IK_CORE_WARN("    File Path : {0}", m_Path);
    IK_CORE_WARN("    Mesh Name : {0}", m_Name);
    IK_CORE_WARN("    Entity ID : {0}", m_EntityId);
    IK_LOG_SEPARATOR();
}

/// Draw the mesh
void Mesh::Draw(const MeshCamera& camera, const glm::mat4& transform) {
    m_BaseMaterial->Set("u_ViewProjection", camera.ViewProjection);
    m_BaseMaterial->Set("u_CameraPosition", camera.Position);

    const auto& material = m_MeshMaterials[m_CurrentMaterialIndex].MaterialInstance;
    for (SubMesh& submesh : m_Submeshes) {
        material->Set("u_Transform", transform * submesh.Transform);
        
        material->Bind();
        
        if (m_IsAnimated) {
            for (size_t i = 0; i < m_BoneTransforms.size(); i++) {
                std::string uniformName = std::string("u_BoneTransforms[") + std::to_string(i) + std::string("]");
                m_BaseMaterial->GetShader()->Bind();
                m_BaseMaterial->GetShader()->SetUniformMat4(uniformName, m_BoneTransforms[i]);
            }
        }
        
        Renderer::DrawIndexedBaseVertex(m_Pipeline, submesh.IndexCount, (void*)(sizeof(uint32_t) * submesh.BaseIndex), submesh.BaseVertex);
        material->Unbind();
    }
    
    RendererStatistics::Get().VertexCount += m_StaticVertices.size();
    RendererStatistics::Get().IndexCount += m_Indices.size();
}

/// Update the Mesh each frame
/// @param ts time step
void Mesh::Update(Timestep ts) {
    if (m_IsAnimated) {
        if (m_AnimationPlaying) {
            m_WorldTime += ts;
            
            float ticksPerSecond = (float)(m_TicksPerSecond != 0 ? m_TicksPerSecond : 25.0f) * m_TimeMultiplier;
            m_AnimationTime += ts * ticksPerSecond;
            m_AnimationTime = fmod(m_AnimationTime, (float)m_Duration);
        }
        
        // TODO: We only need to recalc bones if rendering has been requested at the current animation frame
        BoneTransform(m_AnimationTime);
    }
}

/// Load the Mesh
void Mesh::LoadMesh(bool loadPredefinedMaterial) {
    // read file via ASSIMP
    Assimp::Importer importer;
    m_Scene = importer.ReadFile(m_Path, s_MeshImportFlags);
    // check for errors
    IK_CORE_ASSERT(!(!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode), importer.GetErrorString());

    m_IsAnimated = m_Scene->mAnimations != nullptr;

    StoreVerticesAndIndices();
    TraverseNodes(m_Scene->mRootNode);
    
    if (m_IsAnimated)
        AddBones();
    
    LoadGraphicsdata();
    
    if (m_IsAnimated) {
        m_BaseMaterial = Material::Create(AssetManager::GetCoreAsset("shaders/3D/PBRAnimShader.glsl"));
        m_InverseTransform = glm::inverse(MeshUtils::Mat4FromAssimpMat4(m_Scene->mRootNode->mTransformation));
        
        // Deep copy animation data as pointer copy is creating mess
        
        // Copy the Animation

        m_Animation = m_Scene->mAnimations[0];
        
        m_TicksPerSecond = m_Animation->mTicksPerSecond;
        m_Duration = m_Animation->mDuration;
        m_NumChannels = m_Animation->mNumChannels;
        
        // Copy aniamtion node tree : Deep copy
        CopyRootNode(m_Scene->mRootNode, &m_RootNode);
        
        // Store each animation channel
        m_AnimChannels = new aiNodeAnim*[m_NumChannels];
        for (uint32_t i = 0; i < m_NumChannels; i++) {
            m_AnimChannels[i] = new aiNodeAnim();
            m_AnimChannels[i]->mNodeName  = m_Animation->mChannels[i]->mNodeName;
            
            m_AnimChannels[i]->mNumPositionKeys = m_Animation->mChannels[i]->mNumPositionKeys;
            m_AnimChannels[i]->mPositionKeys = new aiVectorKey[m_AnimChannels[i]->mNumPositionKeys];
            
            for (uint32_t j = 0; j < m_AnimChannels[i]->mNumPositionKeys; j++) {
                m_AnimChannels[i]->mPositionKeys[j].mTime = m_Animation->mChannels[i]->mPositionKeys[j].mTime;
                m_AnimChannels[i]->mPositionKeys[j].mValue = m_Animation->mChannels[i]->mPositionKeys[j].mValue;
            }
            
            m_AnimChannels[i]->mNumRotationKeys = m_Animation->mChannels[i]->mNumRotationKeys;
            m_AnimChannels[i]->mRotationKeys = new aiQuatKey[m_AnimChannels[i]->mNumRotationKeys];
            for (uint32_t j = 0; j < m_AnimChannels[i]->mNumRotationKeys; j++) {
                m_AnimChannels[i]->mRotationKeys[j].mTime = m_Animation->mChannels[i]->mRotationKeys[j].mTime;
                m_AnimChannels[i]->mRotationKeys[j].mValue = m_Animation->mChannels[i]->mRotationKeys[j].mValue;
            }
            
            m_AnimChannels[i]->mNumScalingKeys = m_Animation->mChannels[i]->mNumScalingKeys;
            m_AnimChannels[i]->mScalingKeys = new aiVectorKey[m_AnimChannels[i]->mNumScalingKeys];
            for (uint32_t j = 0; j < m_AnimChannels[i]->mNumScalingKeys; j++) {
                m_AnimChannels[i]->mScalingKeys[j].mTime = m_Animation->mChannels[i]->mScalingKeys[j].mTime;
                m_AnimChannels[i]->mScalingKeys[j].mValue = m_Animation->mChannels[i]->mScalingKeys[j].mValue;
            }
            
            m_AnimChannels[i]->mPreState = m_Animation->mChannels[i]->mPreState;
            m_AnimChannels[i]->mPostState = m_Animation->mChannels[i]->mPostState;
        }
    }
    else {
        m_BaseMaterial = Material::Create(AssetManager::GetCoreAsset("shaders/3D/PBRStaticShader.glsl"));
    }
    
    if (loadPredefinedMaterial)
        UploadMaterials();
}

/// Stores vertices and Indices to Graphics buffer
void Mesh::StoreVerticesAndIndices() {
    IK_CORE_INFO("    ---------------------------------------------");
    IK_CORE_INFO("Storing Vertices and Indices for : {0}", m_Name);
    IK_CORE_INFO("    Number of Submesh         : {0}",  m_Scene->mNumMeshes);

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    
    m_BoundingBox.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
    m_BoundingBox.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

    m_Submeshes.reserve(m_Scene->mNumMeshes);
    for (size_t m = 0; m < m_Scene->mNumMeshes; m++) {
        aiMesh* mesh = m_Scene->mMeshes[m];

        SubMesh& submesh = m_Submeshes.emplace_back();
        submesh.BaseVertex = vertexCount;
        submesh.BaseIndex = indexCount;
        submesh.MaterialIndex = mesh->mMaterialIndex;
        submesh.IndexCount = mesh->mNumFaces * 3;
        submesh.MeshName = mesh->mName.C_Str();
        
        vertexCount += mesh->mNumVertices;
        indexCount += submesh.IndexCount;

        IK_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
        IK_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");
        
        auto& aabb = submesh.BoundingBox;
        aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
        aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
        
        // Vertices
        if (m_IsAnimated) {
            for (size_t i = 0; i < mesh->mNumVertices; i++) {
                AnimatedVertex vertex;
                vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
                vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
                
                aabb.Min.x = glm::min(vertex.Position.x, aabb.Min.x);
                aabb.Min.y = glm::min(vertex.Position.y, aabb.Min.y);
                aabb.Min.z = glm::min(vertex.Position.z, aabb.Min.z);
                aabb.Max.x = glm::max(vertex.Position.x, aabb.Max.x);
                aabb.Max.y = glm::max(vertex.Position.y, aabb.Max.y);
                aabb.Max.z = glm::max(vertex.Position.z, aabb.Max.z);

                if (mesh->HasTangentsAndBitangents()) {
                    vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
                    vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
                }
                
                if (mesh->HasTextureCoords(0))
                    vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                
                vertex.ObjectID = m_EntityId;
                
                m_AnimatedVertices.push_back(vertex);
            }
        }
        else {
            for (size_t i = 0; i < mesh->mNumVertices; i++) {
                StaticVertex vertex;
                vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
                vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
                
                aabb.Min.x = glm::min(vertex.Position.x, aabb.Min.x);
                aabb.Min.y = glm::min(vertex.Position.y, aabb.Min.y);
                aabb.Min.z = glm::min(vertex.Position.z, aabb.Min.z);
                aabb.Max.x = glm::max(vertex.Position.x, aabb.Max.x);
                aabb.Max.y = glm::max(vertex.Position.y, aabb.Max.y);
                aabb.Max.z = glm::max(vertex.Position.z, aabb.Max.z);

                if (mesh->HasTangentsAndBitangents()) {
                    vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
                    vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
                }
                
                if (mesh->HasTextureCoords(0))
                    vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

                vertex.ObjectID = m_EntityId;
                m_StaticVertices.push_back(vertex);
            }
            
            m_BoundingBox.Min.x = glm::min(aabb.Min.x, m_BoundingBox.Min.x);
            m_BoundingBox.Min.y = glm::min(aabb.Min.y, m_BoundingBox.Min.y);
            m_BoundingBox.Min.z = glm::min(aabb.Min.z, m_BoundingBox.Min.z);
            m_BoundingBox.Max.x = glm::max(aabb.Max.x, m_BoundingBox.Max.x);
            m_BoundingBox.Max.y = glm::max(aabb.Max.y, m_BoundingBox.Max.y);
            m_BoundingBox.Max.z = glm::max(aabb.Max.z, m_BoundingBox.Max.z);
        }

        // Indices
        for (size_t i = 0; i < mesh->mNumFaces; i++) {
            IK_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
            Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
            m_Indices.push_back(index);
            
            if (!m_IsAnimated)
                m_TriangleCache[(uint32_t)m].emplace_back(m_StaticVertices[index.V1 + submesh.BaseVertex], m_StaticVertices[index.V2 + submesh.BaseVertex], m_StaticVertices[index.V3 + submesh.BaseVertex]);
        }
    } // for (size_t m = 0; m < m_Scene->mNumMeshes; m++)
    
    if (m_IsAnimated)
        IK_CORE_INFO("    Number of Animated Vertex : {0}", m_AnimatedVertices.size());
    else
        IK_CORE_INFO("    Number of Static Vertex   : {0}", m_StaticVertices.size());

    IK_CORE_INFO("    Number of Indices         : {0}", m_Indices.size());
}

/// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process
/// on its children nodes (if any).
/// @param node node pointer of submesh
/// @param parentTransform parent node transform
/// @param level level
void Mesh::TraverseNodes(aiNode* node, const glm::mat4& parentTransform, uint32_t level) {
    glm::mat4 transform = parentTransform * MeshUtils::Mat4FromAssimpMat4(node->mTransformation);
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        uint32_t mesh = node->mMeshes[i];
        auto& submesh = m_Submeshes[mesh];
        submesh.NodeName = node->mName.C_Str();
        submesh.Transform = transform;
    }
    
    for (uint32_t i = 0; i < node->mNumChildren; i++)
        TraverseNodes(node->mChildren[i], transform, level + 1);
}

/// Add Bones in Mesh
void Mesh::AddBones() {
    IK_CORE_INFO("    ---------------------------------------------");
    IK_CORE_INFO("Adding Bones in Mesh : {0}", m_Name);
    for (size_t m = 0; m < m_Scene->mNumMeshes; m++) {
        aiMesh* mesh = m_Scene->mMeshes[m];
        SubMesh& submesh = m_Submeshes[m];
        
        for (size_t i = 0; i < mesh->mNumBones; i++) {
            aiBone* bone = mesh->mBones[i];
            std::string boneName(bone->mName.data);
            int boneIndex = 0;
            
            if (m_BoneMapping.find(boneName) == m_BoneMapping.end()) {
                // Allocate an index for a new bone
                boneIndex = m_BoneCount;
                m_BoneCount++;
                
                BoneInfo bi;
                m_BoneInfo.push_back(bi);
                m_BoneInfo[boneIndex].BoneOffset = MeshUtils::Mat4FromAssimpMat4(bone->mOffsetMatrix);
                m_BoneMapping[boneName] = boneIndex;
            }
            else {
                IK_CORE_INFO("    Found existing bone in map");
                boneIndex = m_BoneMapping[boneName];
            }
            
            for (size_t j = 0; j < bone->mNumWeights; j++) {
                int32_t VertexID = submesh.BaseVertex + bone->mWeights[j].mVertexId;
                float Weight = bone->mWeights[j].mWeight;
                m_AnimatedVertices[VertexID].AddBoneData(boneIndex, Weight);
            }
        }
    }
    m_BoneTransforms.resize(m_BoneCount);
    IK_CORE_INFO("    Number of Bones : {0}", m_BoneCount);
}

/// Load all stored data to Graphics Buffer and array
void Mesh::LoadGraphicsdata() {
    IK_CORE_INFO("    ---------------------------------------------");
    IK_CORE_INFO("Storing Data to Renderer Buffers for : {0}", m_Name);
    
    m_Pipeline = Pipeline::Create();

    if (m_IsAnimated) {
        m_VertexBuffer = VertexBuffer::Create((void*)(m_AnimatedVertices.data()), (uint32_t)(m_AnimatedVertices.size() * sizeof(AnimatedVertex)));
        m_VertexBuffer->AddLayout({
            { "a_Position", ShaderDataType::Float3 },
            { "a_Normal", ShaderDataType::Float3 },
            { "a_Tangent", ShaderDataType::Float3 },
            { "a_Binormal", ShaderDataType::Float3 },
            { "a_TexCoord", ShaderDataType::Float2 },
            { "a_BoneIDs", ShaderDataType::Int4 },
            { "a_BoneWeights", ShaderDataType::Float4 },
            { "a_ObjectId", ShaderDataType::Int },
        });
    }
    else {
        m_VertexBuffer = VertexBuffer::Create((void*)(m_StaticVertices.data()), uint32_t(m_StaticVertices.size() * sizeof(StaticVertex)));
        m_VertexBuffer->AddLayout({
            { "a_Position",  ShaderDataType::Float3 },
            { "a_Normal",    ShaderDataType::Float3 },
            { "a_TexCoord",  ShaderDataType::Float2 },
            { "a_Tangent",   ShaderDataType::Float3 },
            { "a_Bitangent", ShaderDataType::Float3 },
            { "a_ObjectID",  ShaderDataType::Int },
        });
    }
    m_Pipeline->AddVertexBuffer(m_VertexBuffer);
    
    m_IndexBuffer = IndexBuffer::CreateWithSize((void*)m_Indices.data(), uint32_t(m_Indices.size() * sizeof(Index)));
    m_Pipeline->SetIndexBuffer(m_IndexBuffer);
}

/// Upload the materials of submesh in Material instance
void Mesh::UploadMaterials() {
    if (m_Scene->HasMaterials()) {
        IK_CORE_INFO("    ---------------------------------------------");
        IK_CORE_INFO("Loading Materials");
        IK_CORE_INFO("    Mesh                : {0}", m_Name);
        IK_CORE_INFO("    Number of Materials : {0}", m_Scene->mNumMaterials);

        m_MeshMaterials.resize(m_Scene->mNumMaterials);
        for (uint32_t i = 0; i < m_Scene->mNumMaterials; i++) {
            auto aiMaterial = m_Scene->mMaterials[i];
            auto aiMaterialName = aiMaterial->GetName();

            m_MeshMaterials[i].MaterialInstance = MaterialInstance::Create(m_BaseMaterial, aiMaterialName.data);
            m_MeshMaterials[i].Name = aiMaterialName.data;

            m_MaterialNames.emplace_back(m_MeshMaterials[i].Name.c_str());
            
            IK_CORE_INFO("    ---------------------------------------------");
            IK_CORE_INFO("        Name      : {0}", aiMaterialName.data);
            IK_CORE_INFO("        Index     : {0}", i);
            
            aiString aiTexPath;
            aiColor3D aiColor;

            aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);

            float shininess, metalness;
            if (aiMaterial->Get(AI_MATKEY_SHININESS, shininess) != aiReturn_SUCCESS)
                shininess = 80.0f; // Default value

            if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
                metalness = 0.0f;

            float roughness = 1.0f - glm::sqrt(shininess / 100.0f);
            IK_CORE_INFO("        Color     : {0} | {1} | {2}", aiColor.r, aiColor.g, aiColor.b);
            IK_CORE_INFO("        Metalness : {0}", metalness);
            IK_CORE_INFO("        Roughness : {0}", roughness);

            m_MeshMaterials[i].Property.AlbedoColor = { 1.0f, aiColor.g, aiColor.b };
            m_MeshMaterials[i].Property.Metalness = metalness;
            m_MeshMaterials[i].Property.Roughness = roughness;

            // Upload the Material Property to shader
            m_MeshMaterials[i].MaterialInstance->Set("u_Material", m_MeshMaterials[i].Property);
            m_MeshMaterials[i].MaterialInstance->Set("u_TextureInvertVertical", 0.0f);

            IK_CORE_INFO("        -----------------------------------------");
            IK_CORE_INFO("            Uploading Textures");
            for (size_t texIdx = 0; texIdx < MaxPBRTextureSupported; texIdx++) {
                IK_CORE_INFO("            -------------------------------------");
                if (aiMaterial->GetTexture(s_AiMaterialType[texIdx], 0, &aiTexPath) == AI_SUCCESS) {
                    std::filesystem::path path = m_Path;
                    auto parentPath = path.parent_path();

                    std::string tempPathString = std::string(aiTexPath.data);
                    std::replace( tempPathString.begin(), tempPathString.end(), '\\', '/'); // replace all 'x' to 'y'

                    parentPath /= tempPathString;
                    std::string texturePath = parentPath.string();
                    
                    IK_CORE_INFO("            Name : {0}", s_Texname[texIdx]);
                    IK_CORE_INFO("            Path : {0}", texturePath);

                    auto texture = Renderer::GetTexture(texturePath);
                    m_MeshMaterials[i].Textures[texIdx] = { texture, true };

                    auto textureShaderName = "u_" + s_Texname[texIdx] + "Texture";

                    m_MeshMaterials[i].MaterialInstance->Set(textureShaderName, texture);
                    m_MeshMaterials[i].MaterialInstance->Set(textureShaderName + "Toggle", 1.0f);
                    
                }
                else {
                    IK_CORE_INFO("            No Noxture for {0}", s_Texname[texIdx]);
                    auto textureShaderName = "u_" + s_Texname[texIdx] + "TextureToggle";
                    m_MeshMaterials[i].Textures[texIdx] = { nullptr, false };
                    m_MeshMaterials[i].MaterialInstance->Set(textureShaderName, 0.0f);
                }
            }

//            IK_MESH_LOG("             Material Property");
//            for (uint32_t i = 0; i < aiMaterial->mNumProperties; i++) {
//                auto prop = aiMaterial->mProperties[i];
//                IK_MESH_LOG("                Name: {0}, Type: {1}, Size: {2}, Value: {3}, Sementic {4}", prop->mKey.data, prop->mType, prop->mDataLength, *(float*)prop->mData, GetSementicType(prop->mSemantic));
//            }
        } // for (uint32_t i = 0; i < m_Scene->mNumMaterials; i++)
    } // if (m_Scene->HasMaterials())
}

/// Update bone transform each frame
/// @param time time
void Mesh::BoneTransform(float time) {
    ReadNodeHierarchy(time, m_RootNode, glm::mat4(1.0f));
    for (size_t i = 0; i < m_BoneCount; i++)
        m_BoneTransforms[i] = m_BoneInfo[i].FinalTransformation;
}

/// Read None hierarchy for each bone
/// @param AnimationTime Animation Time
/// @param pNode Node pointer
/// @param parentTransform parent node transform matrix
void Mesh::ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& parentTransform) {
    std::string name(pNode->mName.data);
    glm::mat4 nodeTransform(MeshUtils::Mat4FromAssimpMat4(pNode->mTransformation));
    const aiNodeAnim* nodeAnim = FindNodeAnim(name);
    
    if (nodeAnim) {
        glm::vec3 translation = InterpolateTranslation(AnimationTime, nodeAnim);
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));
        
        glm::quat rotation = InterpolateRotation(AnimationTime, nodeAnim);
        glm::mat4 rotationMatrix = glm::toMat4(rotation);
        
        glm::vec3 scale = InterpolateScale(AnimationTime, nodeAnim);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z));
        
        nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
    }
    
    glm::mat4 transform = parentTransform * nodeTransform;
    
    if (m_BoneMapping.find(name) != m_BoneMapping.end()) {
        uint32_t BoneIndex = m_BoneMapping[name];
        m_BoneInfo[BoneIndex].FinalTransformation = m_InverseTransform * transform * m_BoneInfo[BoneIndex].BoneOffset;
    }
    
    for (uint32_t i = 0; i < pNode->mNumChildren; i++)
        ReadNodeHierarchy(AnimationTime, pNode->mChildren[i], transform);
}

/// Fine node animation
/// @param nodeName node name
const aiNodeAnim* Mesh::FindNodeAnim(const std::string& nodeName) {
    for (uint32_t i = 0; i < m_NumChannels; i++) {
        const aiNodeAnim* nodeAnim = m_AnimChannels[i];
        if (std::string(nodeAnim->mNodeName.data) == nodeName)
            return nodeAnim;
    }
    return nullptr;
}

/// Interpolate Node translation
/// @param animationTime animation time
/// @param nodeAnim animation pointer
glm::vec3 Mesh::InterpolateTranslation(float animationTime, const aiNodeAnim* nodeAnim) {
    if (nodeAnim->mNumPositionKeys == 1) {
        // No interpolation necessary for single value
        auto v = nodeAnim->mPositionKeys[0].mValue;
        return { v.x, v.y, v.z };
    }
    
    uint32_t PositionIndex = FindPosition(animationTime, nodeAnim);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    
    IK_CORE_ASSERT(NextPositionIndex < nodeAnim->mNumPositionKeys);
    
    float DeltaTime = (float)(nodeAnim->mPositionKeys[NextPositionIndex].mTime - nodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (animationTime - (float)nodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    
    IK_CORE_ASSERT(Factor <= 1.0f, "Factor must be below 1.0f");
    Factor = glm::clamp(Factor, 0.0f, 1.0f);
    
    const aiVector3D& Start = nodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = nodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    auto aiVec = Start + Factor * Delta;
    
    return { aiVec.x, aiVec.y, aiVec.z };
}

/// Interpolate Node Rotation
/// @param animationTime animation time
/// @param nodeAnim animation pointer
glm::quat Mesh::InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim) {
    if (nodeAnim->mNumRotationKeys == 1) {
        // No interpolation necessary for single value
        auto v = nodeAnim->mRotationKeys[0].mValue;
        return glm::quat(v.w, v.x, v.y, v.z);
    }
    
    uint32_t RotationIndex = FindRotation(animationTime, nodeAnim);
    uint32_t NextRotationIndex = (RotationIndex + 1);
    
    IK_CORE_ASSERT(NextRotationIndex < nodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(nodeAnim->mRotationKeys[NextRotationIndex].mTime - nodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (animationTime - (float)nodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    
    IK_CORE_ASSERT(Factor <= 1.0f, "Factor must be below 1.0f");
    Factor = glm::clamp(Factor, 0.0f, 1.0f);
    
    const aiQuaternion& StartRotationQ = nodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = nodeAnim->mRotationKeys[NextRotationIndex].mValue;
    auto q = aiQuaternion();
    aiQuaternion::Interpolate(q, StartRotationQ, EndRotationQ, Factor);
    q = q.Normalize();
    
    return glm::quat(q.w, q.x, q.y, q.z);
}

/// Interpolate Node Scale
/// @param animationTime animation time
/// @param nodeAnim animation pointer
glm::vec3 Mesh::InterpolateScale(float animationTime, const aiNodeAnim* nodeAnim) {
    if (nodeAnim->mNumScalingKeys == 1) {
        // No interpolation necessary for single value
        auto v = nodeAnim->mScalingKeys[0].mValue;
        return { v.x, v.y, v.z };
    }
    
    uint32_t index = FindScaling(animationTime, nodeAnim);
    uint32_t nextIndex = (index + 1);
    
    IK_CORE_ASSERT(nextIndex < nodeAnim->mNumScalingKeys);
    float deltaTime = (float)(nodeAnim->mScalingKeys[nextIndex].mTime - nodeAnim->mScalingKeys[index].mTime);
    float factor = (animationTime - (float)nodeAnim->mScalingKeys[index].mTime) / deltaTime;
    
    IK_CORE_ASSERT(factor <= 1.0f, "Factor must be below 1.0f");
    factor = glm::clamp(factor, 0.0f, 1.0f);
    
    const auto& start = nodeAnim->mScalingKeys[index].mValue;
    const auto& end = nodeAnim->mScalingKeys[nextIndex].mValue;
    auto delta = end - start;
    auto aiVec = start + factor * delta;
    
    return { aiVec.x, aiVec.y, aiVec.z };
}

/// Find Position of anumated node
/// @param AnimationTime animation time
/// @param pNodeAnim animation pointer
uint32_t Mesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim) {
    for (uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
            return i;
    }
    return 0;
}

/// Find Rotation of anumated node
/// @param AnimationTime animation time
/// @param pNodeAnim animation pointer
uint32_t Mesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim) {
    IK_CORE_ASSERT(pNodeAnim->mNumRotationKeys > 0);
    for (uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
            return i;
    }
    return 0;
}

/// Find Scale of anumated node
/// @param AnimationTime animation time
/// @param pNodeAnim animation pointer
uint32_t Mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim) {
    IK_CORE_ASSERT(pNodeAnim->mNumScalingKeys > 0);
    
    for (uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
            return i;
    }
    return 0;
}

/// Deep copy root node recursive
/// @param pSrcNode Source node
/// @param pDstNode destination Node
void Mesh::CopyRootNode(const aiNode* pSrcNode, aiNode** pDstNode) {
    *pDstNode = new aiNode;
    (*pDstNode)->mName = pSrcNode->mName;
    (*pDstNode)->mTransformation = pSrcNode->mTransformation;
    
    (*pDstNode)->mNumChildren = pSrcNode->mNumChildren;
    ((*pDstNode)->mChildren) = new aiNode*[pSrcNode->mNumChildren];
    
    for (uint32_t i = 0; i < pSrcNode->mNumChildren; i++)
        CopyRootNode(pSrcNode->mChildren[i], &(*pDstNode)->mChildren[i]);
}

/// Add a material in the mesh
/// @param name Name of Material
/// @param prop Property of Material
/// @param texComp Array of texture for material
/// @param invertX is inverted the textures Horizonally
/// @param invertY is inverted the textures Vertically
void Mesh::AddMaterial(const std::string& name, const MaterialProperty& prop, const std::array<TextureComponent, MaxPBRTextureSupported>& texComp, bool invertX, bool invertY) {
    if (std::find(m_MaterialNames.begin(), m_MaterialNames.end(), name) != m_MaterialNames.end())
        return;
    
    MeshMaterial meshMaterial;
    meshMaterial.Name = name;
    meshMaterial.Property = prop;
    meshMaterial.Textures = texComp;
    
    meshMaterial.InvertTextureX = invertX;
    meshMaterial.InvertTextureY = invertY;
    
    meshMaterial.MaterialInstance = MaterialInstance::Create(m_BaseMaterial, name);
    meshMaterial.MaterialInstance->Set("u_Material", meshMaterial.Property);
    meshMaterial.MaterialInstance->Set("u_TextureInvertHorizontal", (float)meshMaterial.InvertTextureX);
    meshMaterial.MaterialInstance->Set("u_TextureInvertVertical", (float)meshMaterial.InvertTextureY);
        
    for (uint32_t texIdx = 0; texIdx < MaxPBRTextureSupported; texIdx++) {
        auto textureShaderName = "u_" + s_Texname[texIdx] + "Texture";

        meshMaterial.MaterialInstance->Set(textureShaderName, texComp[texIdx].Component);
        meshMaterial.MaterialInstance->Set(textureShaderName + "Toggle", (float)texComp[texIdx].Use);
    }

    m_MeshMaterials.emplace_back(meshMaterial);
    m_MaterialNames.emplace_back(name);
}

/// Redner Imgui for Mesh
/// @param defaultTexture Default Texture
void Mesh::RenderImgui(const std::shared_ptr<Texture>& defaultTexture) {
    ImGui::PushID(m_Path.c_str());
    
    // Add or remove the material from Mesh
    static bool addMaterial = false, removeMaterial = false;
    if (addMaterial) {
        MeshMaterial meshMaterial;
        meshMaterial.MaterialInstance = MaterialInstance::Create(m_BaseMaterial, "Unnamed Material");
        meshMaterial.MaterialInstance->Set("u_Material", meshMaterial.Property);
        
        m_MeshMaterials.emplace_back(meshMaterial);
        m_MaterialNames.emplace_back("Unnamed Material");
        
        addMaterial = false;
    }
    if (removeMaterial) {
        m_MaterialNames.erase(m_MaterialNames.begin() + m_CurrentMaterialIndex);
        m_MeshMaterials.erase(m_MeshMaterials.begin() + m_CurrentMaterialIndex);
        
        m_CurrentMaterialIndex = 0;
        removeMaterial = false;
    }

    // Drop option to set active material
    m_CurrentMaterialIndex = PropertyGrid::ComboDrop("Active Material", m_MaterialNames, m_CurrentMaterialIndex);
    
    MeshMaterial& meshMaterialProp = m_MeshMaterials[m_CurrentMaterialIndex];
        
    // Shoes tree node of current selected material
    static const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    bool opened = ImGui::TreeNodeEx((void*)m_Path.c_str(), treeNodeFlags, m_MaterialNames[m_CurrentMaterialIndex].c_str());

    // Button to show option to Add or Remove Materials from mesh
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 0.5f);
    if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
        ImGui::OpenPopup("Material Setting");
    
    if (ImGui::BeginPopup("Material Setting")) {
        if (ImGui::MenuItem("Add Material"))
            addMaterial = true;
        ImGui::Separator();
        if (ImGui::MenuItem("Remove Material", nullptr, false, (m_MeshMaterials.size() > 1)))
            removeMaterial = true;
        ImGui::EndPopup();
    }
    
    // Show Material conentns
    if (opened) {
        auto& material = meshMaterialProp.MaterialInstance;
        
        // Rename the material
        if (PropertyGrid::String("Material Name", meshMaterialProp.Name, 100.0f, 300.0f)) {
            m_MaterialNames[m_CurrentMaterialIndex] = meshMaterialProp.Name.c_str();
            m_MeshMaterials[m_CurrentMaterialIndex].MaterialInstance->SetName(meshMaterialProp.Name.c_str());
        }
        
        ImGui::Separator();

        // Invert the texture
        ImGui::Columns(3);
        ImGui::Text("Invert Texture");
        ImGui::NextColumn();
        if (ImGui::Checkbox("X", &meshMaterialProp.InvertTextureX))
            material->Set("u_TextureInvertHorizontal", (float)meshMaterialProp.InvertTextureX);
        ImGui::NextColumn();
        if (ImGui::Checkbox("Y", &meshMaterialProp.InvertTextureY))
            material->Set("u_TextureInvertVertical", (float)meshMaterialProp.InvertTextureY);
        ImGui::Columns(1);
        
        ImGui::Separator();

        // Shows all the texture info for current material
        int32_t textureIndex = 0;
        for (const auto& name : s_Texname) {
            ImGui::PushID(textureIndex);
            ImGui::Columns(2);
            
            ImGui::SetColumnWidth(0, 100);
            auto& texture = meshMaterialProp.Textures[textureIndex];

            size_t texId = ((texture.Component) ? texture.Component->GetRendererID() : defaultTexture->GetRendererID());
            ImGui::Image((void*)texId, ImVec2(40.0f, 40.0f), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
            PropertyGrid::DropConent([&texture, &material, textureIndex](const std::string& path)
                                     {
                texture.Component.reset();
                texture.Component = Texture::Create(path);
                material->Set(s_ShaderTextureName[textureIndex], texture.Component);
            });
            
            ImGui::NextColumn();
            
            if (ImGui::Checkbox(name.c_str(), &texture.Use)) {
                auto textureShaderName = s_ShaderTextureName[textureIndex] + "Toggle";
                material->Set(textureShaderName, (float)texture.Use);
            }
            
            ImGui::SameLine();
            PropertyGrid::HelpMarker("Drop the Texture file in the Image Button to upload the texture or Select already stored texture from the scene (Option can be available by right click on image)");
            
            // If Texture is not enale then show prperty data else show Invert texture flag
            if (!texture.Use) {
                bool modified = false;
                if (name == "Albedo")
                    modified = ImGui::ColorEdit4("Color ", glm::value_ptr(meshMaterialProp.Property.AlbedoColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                else if (name == "Roughness")
                    modified = ImGui::DragFloat("", &meshMaterialProp.Property.Roughness, 0.01f, 0.0f, 1.0f);
                else if (name == "Metallic")
                    modified = ImGui::DragFloat("", &meshMaterialProp.Property.Metalness, 0.01f, 0.0f, 1.0f);
                else
                    ImGui::NewLine();
                
                if (modified)
                    material->Set("u_Material", meshMaterialProp.Property);
            }
            else {
                ImGui::NewLine();
            }
            
            ImGui::Separator();

            ImGui::PopID();
            ImGui::Columns(1);
            textureIndex++;
        }
        
        ImGui::TreePop();
    }
    ImGui::PopID();
}

const std::string& Mesh::GetName() const { return m_Name; }
const std::string& Mesh::GetPath() const { return m_Path; }
const std::vector<MeshMaterial>& Mesh::GetMaterials() const { return m_MeshMaterials; }
const uint32_t Mesh::GetActiveMaterialIndex() const { return m_CurrentMaterialIndex; }
void Mesh::SetActiveMaterialIndex(uint32_t currMatIndex) { m_CurrentMaterialIndex = currMatIndex; }
