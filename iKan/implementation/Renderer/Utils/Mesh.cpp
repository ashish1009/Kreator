//
//  Mesh.cpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#include "Mesh.hpp"

// Assimp Files
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace iKan;

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
    IK_CORE_WARN("            Vertex has more than four bones/weights affecting it, extra data will be discarded (BoneID : {0}, Weight :{1})", BoneID, Weight);
}

/// Create mesh API
/// @param path Mesh File path
/// @param entityId Entity ID of Mesh
std::shared_ptr<Mesh> Mesh::Create(const std::string& path, uint32_t entityId) {
    return std::make_shared<Mesh>(path, entityId);
}

/// Mesh Constructor
/// @param path File Path
Mesh::Mesh(const std::string& path, uint32_t entityId)
: m_Path(path), m_Directory(StringUtils::GetDirectoryFromFilePath(path)), m_Name(StringUtils::GetNameFromFilePath(path)), m_EntityId(entityId) {
    PROFILE();
    
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Mesh ...");
    IK_CORE_INFO("    File Path : {0}", m_Path);
    IK_CORE_INFO("    Mesh Name : {0}", m_Name);
    IK_CORE_INFO("    Entity ID : {0}", m_EntityId);
    
    LoadMesh();
    
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

/// Load the Mesh
void Mesh::LoadMesh() {
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
}

/// Stores vertices and Indices to Graphics buffer
void Mesh::StoreVerticesAndIndices() {
    IK_CORE_INFO("        -----------------------------------------");
    IK_CORE_INFO("        Storing Vertices and Indices");
    IK_CORE_INFO("            Number of Submesh: {0}",  m_Scene->mNumMeshes);

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
        IK_CORE_INFO("            Number of Animated Vertex : {0}", m_AnimatedVertices.size());
    else
        IK_CORE_INFO("            Number of Static Vertex   : {0}", m_StaticVertices.size());

    IK_CORE_INFO("            Number of Indices         : {0}", m_Indices.size());
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
    IK_CORE_INFO("        -----------------------------------------");
    IK_CORE_INFO("        Adding Bones in Mesh");
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
                IK_CORE_INFO("            Found existing bone in map");
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
    IK_CORE_INFO("            Number of Bones : {0}", m_BoneCount);
}

/// Load all stored data to Graphics Buffer and array
void Mesh::LoadGraphicsdata() {
    IK_CORE_INFO("        -----------------------------------------");
    IK_CORE_INFO("        Storing Data to Renderer Buffers");
    
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
