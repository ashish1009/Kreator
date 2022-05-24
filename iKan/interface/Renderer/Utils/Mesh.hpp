//
//  Mesh.hpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#pragma once

#include "Renderer/Graphics/Pipeline.hpp"

class aiScene;
class aiNode;
class aiNodeAnim;
class aiAnimation;

namespace iKan {
    
    /// Vertex of Static Submesh
    struct StaticVertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        int32_t ObjectID;
    };
    
    /// Vertex of Animated Subesh
    struct AnimatedVertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 Tangent;
        glm::vec3 Binormal;
        glm::vec2 Texcoord;
        
        uint32_t IDs[4] = { 0, 0,0, 0 };
        float Weights[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
        
        int32_t ObjectID;
        
        void AddBoneData(uint32_t BoneID, float Weight);
    };
    
    /// Bone Information
    struct BoneInfo {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;
    };
    
    /// Index Infor of each submesh
    struct Index { uint32_t V1, V2, V3; };
    
    /// Triamgele coordinate
    struct Triangle {
        StaticVertex V0, V1, V2;
        Triangle(const StaticVertex& v0, const StaticVertex& v1, const StaticVertex& v2) : V0(v0), V1(v1), V2(v2) {}
    };

    /// Stores submesh content
    struct SubMesh {
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t MaterialIndex;
        uint32_t IndexCount;
        
        AABB BoundingBox;
        glm::mat4 Transform;
        
        std::string NodeName, MeshName;
    };
    
    /// Mesh Loader class
    class Mesh {
    public:
        Mesh(const std::string& path, uint32_t entityId);
        ~Mesh();
        
        /// Create instance of Mesh
        /// @param path Path of Mesh to be loaded
        /// @param entityId Entity ID of Mesh
        static std::shared_ptr<Mesh> Create(const std::string& path, uint32_t entityId);
        
    private:
        // Member Methods
        void LoadMesh();
        void StoreVerticesAndIndices();
        void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);
        void AddBones();
        void LoadGraphicsdata();

        // Member Variables
        AABB m_BoundingBox;
        
        // Graphics
        std::shared_ptr<Pipeline> m_Pipeline;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        
        std::vector<SubMesh> m_Submeshes;
        std::vector<StaticVertex> m_StaticVertices;
        std::vector<AnimatedVertex> m_AnimatedVertices;
        std::vector<Index> m_Indices;

        std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

        // Animation Data
        bool m_IsAnimated = false;

        uint32_t m_BoneCount = 0;

        std::unordered_map<std::string, uint32_t> m_BoneMapping;
        std::vector<BoneInfo> m_BoneInfo;
        std::vector<glm::mat4> m_BoneTransforms;

        // Assimp
        const aiScene* m_Scene;

        // Utils
        uint32_t m_EntityId;
        std::string m_Path, m_Directory, m_Name; // Path stores the exact path with file name, while directory stores only folder path
    };
    
}
