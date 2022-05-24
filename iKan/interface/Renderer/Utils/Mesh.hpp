//
//  Mesh.hpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#pragma once

#include "Core/Utils/Timestep.hpp"
#include "Renderer/Utils/Material.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Renderer/Graphics/Pipeline.hpp"

class aiScene;
class aiNode;
class aiNodeAnim;
class aiAnimation;

namespace iKan {
    
    static constexpr uint32_t MaxPBRTextureSupported = 5;
    
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
    
    /// Material Infor Sent directly to Shader
    struct MaterialProperty {
        glm::vec3 AlbedoColor = glm::vec3(1.0f);
        
        float Metalness = 0.5;;
        float Roughness = 0.5f;
        float TilinghFactor = 1.0f;
    };
    
    /// Mesh Material data
    struct MeshMaterial {
        bool InvertTextureX = false, InvertTextureY = false;
        std::string Name = "Unnamed Mateiral";
        MaterialProperty Property;
        std::shared_ptr<MaterialInstance> MaterialInstance = nullptr;
        std::array<TextureComponent, MaxPBRTextureSupported> Textures;
    };
    
    /// Wrapper structure used to draw mesh. load to SHader
    struct MeshCamera {
        glm::vec3 Position;
        glm::mat4 ViewProjection;
        
        MeshCamera(glm::vec3 position, glm::mat4 viewProjection) : Position(position), ViewProjection(viewProjection) {}
    };
    
    /// Mesh Loader class
    class Mesh {
    public:
        Mesh(const std::string& path, uint32_t entityId, bool loadPredefinedMaterial);
        ~Mesh();
        
        /// Draw the Mesh
        /// @param camera Mesh Camera reference
        /// @param transform mesh transform matrix
        void Draw(const MeshCamera& camera, const glm::mat4& transform);

        /// Update the mesh position each frame : For animation
        /// @param ts time step of each frame
        void Update(Timestep ts);
        
        /// Create instance of Mesh
        /// @param path Path of Mesh to be loaded
        /// @param entityId Entity ID of Mesh
        static std::shared_ptr<Mesh> Create(const std::string& path, uint32_t entityId, bool loadPredefinedMaterial = true);
        
    private:
        // Member Methods
        void LoadMesh(bool loadPredefinedMaterial);
        void StoreVerticesAndIndices();
        void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);
        void AddBones();
        void LoadGraphicsdata();

        void UploadMaterials();;
        
        void BoneTransform(float time);
        void ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& parentTransform);
        
        const aiNodeAnim* FindNodeAnim(const std::string& nodeName);

        uint32_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint32_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint32_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
        glm::vec3 InterpolateTranslation(float animationTime, const aiNodeAnim* nodeAnim);
        glm::quat InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim);
        glm::vec3 InterpolateScale(float animationTime, const aiNodeAnim* nodeAnim);

        void CopyRootNode(const aiNode* pSrcNode, aiNode** pDstNode);
        void DeleteRootNode(aiNode** pDstNode);

        // Member Variables
        AABB m_BoundingBox;
        
        // Graphics
        std::shared_ptr<Pipeline> m_Pipeline;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::shared_ptr<Material> m_BaseMaterial;
        
        std::vector<SubMesh> m_Submeshes;
        std::vector<StaticVertex> m_StaticVertices;
        std::vector<AnimatedVertex> m_AnimatedVertices;
        std::vector<Index> m_Indices;

        std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;
        
        // Material Information
        uint32_t m_CurrentMaterialIndex = 0;
        std::vector<MeshMaterial> m_MeshMaterials;
        std::vector<std::string> m_MaterialNames; // Store material name for GUI

        // Animation Data
        bool m_IsAnimated = false, m_AnimationPlaying = true;
        float m_AnimationTime = 0.0f;
        float m_WorldTime = 0.0f;
        float m_TimeMultiplier = 1.0f;
        float m_TicksPerSecond = 0.0;
        float m_Duration = 0.0;

        uint32_t m_BoneCount = 0;

        aiNode* m_RootNode;

        // For each Animation
        uint32_t m_NumChannels = 0;
        aiAnimation* m_Animation;
        aiNodeAnim** m_AnimChannels;

        glm::mat4 m_InverseTransform;

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
