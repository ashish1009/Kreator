//
//  SceneSerialiser.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "SceneSerialiser.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Component.hpp"
#include "Renderer/Utils/Mesh.hpp"
#include <yaml-cpp/yaml.h>

namespace YAML {
    
    // yml converstions vec2
    template<> struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
        
        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;
            
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };
    
    // yml converstions vec3
    template<> struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
        
        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;
            
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };
    
    // yml converstions
    template<> struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
        
        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4)
                return false;
            
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
    
}

using namespace iKan;

// yml << operator for glm vec 2
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

// yml << operator for glm vec 3
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

// yml << operator for glm vec 4
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
}


/// Serialize the entity. Extract the entity componenets from serialiser
static void SerializeEntity(YAML::Emitter& out, std::shared_ptr<Entity> entity) {
    out << YAML::BeginMap; // Entity
    out << YAML::Key << "Entity" << YAML::Value << entity->GetUUID();
    
    if (entity->HasComponent<TagComponent>()) {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap; // TagComponent
        
        auto& tag = entity->GetComponent<TagComponent>().Tag;
        auto& group = entity->GetComponent<TagComponent>().Group;
        out << YAML::Key << "Tag" << YAML::Value << tag;
        out << YAML::Key << "Group" << YAML::Value << group;
        
        out << YAML::EndMap; // TagComponent
    }
    
    if (entity->HasComponent<TransformComponent>()) {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap; // TransformComponent
        
        auto& tc = entity->GetComponent<TransformComponent>();
        out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
        out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
        out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
        
        out << YAML::EndMap; // TransformComponent
    }
    
    if (entity->HasComponent<CameraComponent>()) {
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap; // CameraComponent
        
        auto& cc = entity->GetComponent<CameraComponent>();
        out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;
        out << YAML::Key << "Primary" << YAML::Value << cc.Primary;

        // Scene Camera
        const auto& camera = cc.Camera;
        out << YAML::Key << "ProjectionType" << YAML::Value << (uint32_t)camera->GetProjectionType();
        out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera->GetPerspectiveFOV();
        out << YAML::Key << "OrthographicSize" << YAML::Value << camera->GetOrthographicSize();
        out << YAML::Key << "Near" << YAML::Value << camera->GetNear();
        out << YAML::Key << "Far" << YAML::Value << camera->GetFar();
        
        out << YAML::EndMap; // CameraComponent
    }
    
    if (entity->HasComponent<QuadComponent>()) {
        out << YAML::Key << "QuadComponent";
        out << YAML::BeginMap; // QuadComponent
        
        auto& qc = entity->GetComponent<QuadComponent>();
        out << YAML::Key << "Color" << YAML::Value << qc.Color;
        out << YAML::Key << "TilingFactor" << YAML::Value << qc.TilingFactor;
        
        if (qc.Texture.Component) {
            out << YAML::Key << "Use" << YAML::Value << qc.Texture.Use;
            out << YAML::Key << "TexturePath" << YAML::Value << qc.Texture.Component->GetfilePath();
        }
        else {
            out << YAML::Key << "Use" << YAML::Value << false;
            out << YAML::Key << "TexturePath" << YAML::Value << "";
        }
        
        out << YAML::EndMap; // QuadComponent
    }
    
    if (entity->HasComponent<CircleComponent>()) {
        out << YAML::Key << "CircleComponent";
        out << YAML::BeginMap; // CircleComponent
        
        auto& qc = entity->GetComponent<CircleComponent>();
        out << YAML::Key << "Color" << YAML::Value << qc.Color;
        out << YAML::Key << "TilingFactor" << YAML::Value << qc.TilingFactor;
        out << YAML::Key << "Thickness" << YAML::Value << qc.Thickness;
        out << YAML::Key << "Fade" << YAML::Value << qc.Fade;

        if (qc.Texture.Component) {
            out << YAML::Key << "Use" << YAML::Value << qc.Texture.Use;
            out << YAML::Key << "TexturePath" << YAML::Value << qc.Texture.Component->GetfilePath();
        }
        else {
            out << YAML::Key << "Use" << YAML::Value << false;
            out << YAML::Key << "TexturePath" << YAML::Value << "";
        }
        
        out << YAML::EndMap; // CircleComponent
    }
    
    if (entity->HasComponent<MeshComponent>()) {
        out << YAML::Key << "MeshComponent";
        out << YAML::BeginMap; // MeshComponent
        
        auto& mesh = entity->GetComponent<MeshComponent>().Mesh;
        if (mesh) {
            out << YAML::Key << "FilePath" << YAML::Value << mesh->GetPath();
            out << YAML::Key << "ActiveMaterial" << YAML::Value << mesh->GetActiveMaterialIndex();
            
            // Mesh Materials
            const auto& meshMaterials = mesh->GetMaterials();
            uint32_t matIdx = 0;
            for (const auto& mat : meshMaterials) {
                auto matIStr = std::to_string(matIdx);
                out << YAML::Key << "Material Name " + matIStr << YAML::Value << mat.Name;
                out << YAML::Key << "Material InvertX " + matIStr << YAML::Value << mat.InvertTextureX;
                out << YAML::Key << "Material InvertY " + matIStr << YAML::Value << mat.InvertTextureY;

                // Material Property
                const auto& prop = mat.Property;
                out << YAML::Key << "Material Proeprty Color " + mat.Name << YAML::Value << prop.AlbedoColor;
                out << YAML::Key << "Material Proeprty Metalness " + mat.Name << YAML::Value << prop.Metalness;
                out << YAML::Key << "Material Proeprty Roughness " + mat.Name << YAML::Value << prop.Roughness;
                out << YAML::Key << "Material Proeprty TilinghFactor " + mat.Name << YAML::Value << prop.TilinghFactor;
                
                // Textures
                uint32_t texIdx = 0;
                for (const auto& texComp : mat.Textures) {
                    auto texIStr = std::to_string(texIdx);
                    out << YAML::Key << "Use " + mat.Name + texIStr << YAML::Value << texComp.Use;
                    if (texComp.Component)
                        out << YAML::Key << "Path " + mat.Name + texIStr << YAML::Value << texComp.Component->GetfilePath();
                    else
                        out << YAML::Key << "Path " + mat.Name + texIStr << YAML::Value << "";
                    texIdx++;
                }
                matIdx++;
            }
            out << YAML::Key << "Num Materials" << YAML::Value << matIdx;
        }
        else {
            out << YAML::Key << "FilePath" << YAML::Value << "";
        }
        out << YAML::EndMap; // MeshComponent
    }
    
    out << YAML::EndMap; // Entity
}


/// SceneSerializer constructor
SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene) : m_Scene(scene) {
}

/// SceneSerializer destructor
SceneSerializer::~SceneSerializer() {
    IK_CORE_INFO("Scene Serialised");
}

/// Serialize the Scene each enityt
void SceneSerializer::Serialize(const std::string& filepath) {
    IK_CORE_INFO("Serialising a Scene");
    IK_CORE_INFO("    Path : {0}", filepath);
    IK_CORE_INFO("    Name : {0}", m_Scene->GetName());
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    
    for (const auto& [uuid, entity] : m_Scene->m_EntityIDMap )
        SerializeEntity(out, entity);

    out << YAML::EndSeq;
    out << YAML::EndMap;
    
    std::ofstream fout(filepath);
    fout << out.c_str();
}

/// Deserialize the Scene Runtime
bool SceneSerializer::Deserialize(const std::string& filepath) {
    YAML::Node data = YAML::LoadFile(filepath);
    
    if (!data["Scene"])
        return false;
    
    std::string sceneName = data["Scene"].as<std::string>();
    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Deserializing scene");
    IK_CORE_INFO("    Path : {0}", filepath);
    IK_CORE_INFO("    Name : {0}", sceneName);

    auto entities = data["Entities"];
    if (entities) {
        for (auto entity : entities) {
            uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO
            
            std::string name, group;
            auto tagComponent = entity["TagComponent"];
            if (tagComponent) {
                name = tagComponent["Tag"].as<std::string>();
                group = tagComponent["Group"].as<std::string>();
            }
            
            std::shared_ptr<Entity> deserializedEntity = m_Scene->CreateEntity(name, uuid);
            IK_CORE_INFO("    Deserialized entity with ID = {0}, name = {1}", uuid, name);
            
            deserializedEntity->GetComponent<TagComponent>().Group = group;
            
            auto transformComponent = entity["TransformComponent"];
            if (transformComponent) {
                // Entities always have transforms
                auto& tc = deserializedEntity->GetComponent<TransformComponent>();
                tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                
                IK_LOG_SEPARATOR();
                IK_CORE_INFO("        Transform Component:");
                IK_CORE_INFO("            Translation: {0}, {1}, {2}", tc.Translation.x, tc.Translation.y, tc.Translation.z);
                IK_CORE_INFO("            Rotation: {0}, {1}, {2}", tc.Rotation.x, tc.Rotation.y, tc.Rotation.z);
                IK_CORE_INFO("            Scale: {0}, {1}, {2}", tc.Scale.x, tc.Scale.y, tc.Scale.z);
            }
            
            auto cameraComponent = entity["CameraComponent"];
            if (cameraComponent) {
                auto type = cameraComponent["ProjectionType"].as<uint32_t>();

                auto& cc = deserializedEntity->AddComponent<CameraComponent>((SceneCamera::ProjectionType)type);
                cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                cc.Primary = cameraComponent["Primary"].as<bool>();
                
                auto fov = cameraComponent["PerspectiveFOV"].as<float>();
                auto size = cameraComponent["OrthographicSize"].as<float>();
                auto near = cameraComponent["Near"].as<float>();
                auto far = cameraComponent["Far"].as<float>();

                if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Orthographic)
                    cc.Camera->SetOrthographic(size, near, far);
                else if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Perspective)
                    cc.Camera->SetPerspective(fov, near, far);
                
                IK_LOG_SEPARATOR();
                IK_CORE_INFO("        Cameara Component:");
                IK_CORE_INFO("            Primary: {0}", cc.Primary);
                IK_CORE_INFO("            Fixed Aspect Ratio: {0}", cc.FixedAspectRatio);
                
                if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Orthographic) {
                    IK_CORE_INFO("            Orthographic Camera :");
                    IK_CORE_INFO("                Orthographic Size: {0}", cc.Camera->GetOrthographicSize());
                }
                else if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Perspective) {
                    IK_CORE_INFO("            Perspective Camera :");
                    IK_CORE_INFO("                Perspective FOV: {0}", cc.Camera->GetPerspectiveFOV());
                }
                
                IK_CORE_INFO("            Near: {0}", cc.Camera->GetNear());
                IK_CORE_INFO("            Far: {0}", cc.Camera->GetFar());
            }
            
            auto quadComponent = entity["QuadComponent"];
            if (quadComponent) {
                const glm::vec4& color = quadComponent["Color"].as<glm::vec4>();
                const float tilingFactor = quadComponent["TilingFactor"].as<float>();
                
                const std::string texFilePath = quadComponent["TexturePath"].as<std::string>();
                const bool use = quadComponent["Use"].as<bool>();

                TextureComponent textComp;
                textComp.Use = use;
                if (texFilePath != "")
                    textComp.Component = Renderer::GetTexture(texFilePath);

                auto& qc = deserializedEntity->AddComponent<QuadComponent>(textComp, color, tilingFactor);

                IK_LOG_SEPARATOR();
                IK_CORE_INFO("        Quad Component:");
                IK_CORE_INFO("            Color: {0}, {1}, {2}, {3}", qc.Color.r, qc.Color.g, qc.Color.b, qc.Color.a);
                IK_CORE_INFO("            Rotation: {0}", qc.TilingFactor);
                
                if (qc.Texture.Component) {
                    IK_CORE_INFO("        Texture Use: {0}", qc.Texture.Use);
                    IK_CORE_INFO("        Texture Path: {0}", qc.Texture.Component->GetfilePath());
                }
            }
  
            auto circleComponent = entity["CircleComponent"];
            if (circleComponent) {
                const glm::vec4& color = circleComponent["Color"].as<glm::vec4>();
                const float tilingFactor = circleComponent["TilingFactor"].as<float>();
                const float thickness = circleComponent["Thickness"].as<float>();
                const float fade = circleComponent["Fade"].as<float>();

                const std::string texFilePath = circleComponent["TexturePath"].as<std::string>();
                const bool use = circleComponent["Use"].as<bool>();
                
                TextureComponent textComp;
                textComp.Use = use;
                if (texFilePath != "")
                    textComp.Component = Renderer::GetTexture(texFilePath);
                
                auto& cc = deserializedEntity->AddComponent<CircleComponent>(textComp, thickness, fade, color, tilingFactor);
                
                IK_CORE_INFO("        Quad Component:");
                IK_CORE_INFO("            Color: {0}, {1}, {2}, {3}", cc.Color.r, cc.Color.g, cc.Color.b, cc.Color.a);
                IK_CORE_INFO("            Rotation: {0}", cc.TilingFactor);
                IK_CORE_INFO("            Thickness: {0}", cc.Thickness);
                IK_CORE_INFO("            Fade: {0}", cc.Fade);
                
                if (cc.Texture.Component) {
                    IK_CORE_INFO("        Texture Use: {0}", cc.Texture.Use);
                    IK_CORE_INFO("        Texture Path: {0}", cc.Texture.Component->GetfilePath());
                }
            }
            
            auto meshComponent = entity["MeshComponent"];
            if (meshComponent) {
                const auto& path = meshComponent["FilePath"].as<std::string>();

                if(path != "") {
                    auto& mc = deserializedEntity->AddComponent<MeshComponent>();
                    mc.Mesh = Mesh::Create(path, (uint32_t)(*deserializedEntity.get()), false);
                    mc.Mesh->SetActiveMaterialIndex(meshComponent["ActiveMaterial"].as<uint32_t>());
                    
                    uint32_t numMaterials = meshComponent["Num Materials"].as<uint32_t>();
                    IK_CORE_INFO("        Mesh Component:");
                    IK_CORE_INFO("            File Path: {0}", mc.Mesh->GetPath());
                    IK_CORE_INFO("            Number of Materials: {0}", numMaterials);

                    for (uint32_t matIdx = 0; matIdx < numMaterials; matIdx++) {
                        auto matIStr = std::to_string(matIdx);
                        auto name = meshComponent["Material Name " + matIStr].as<std::string>();
                        auto invertX = meshComponent["Material InvertX " + matIStr].as<bool>();
                        auto invertY = meshComponent["Material InvertY " + matIStr].as<bool>();
                        IK_CORE_INFO("                Name: {0}", name);
                        
                        MaterialProperty prop;
                        prop.AlbedoColor = meshComponent["Material Proeprty Color " + name].as<glm::vec3>();
                        prop.Metalness = meshComponent["Material Proeprty Metalness " + name].as<float>();
                        prop.Roughness = meshComponent["Material Proeprty Roughness " + name].as<float>();
                        prop.TilinghFactor = meshComponent["Material Proeprty TilinghFactor " + name].as<float>();

                        IK_CORE_INFO("                    Color: {0}, {1}, {2}", prop.AlbedoColor.r, prop.AlbedoColor.g, prop.AlbedoColor.b);
                        IK_CORE_INFO("                    Metalness: {0}", prop.Metalness);
                        IK_CORE_INFO("                    Roughness: {0}", prop.Roughness);
                        IK_CORE_INFO("                    Tiling Factor: {0}", prop.TilinghFactor);

                        std::array<TextureComponent, MaxPBRTextureSupported> textures;
                        for (uint32_t texIdx = 0; texIdx < MaxPBRTextureSupported; texIdx++) {
                            auto texIStr = std::to_string(texIdx);
                            textures[texIdx].Use = meshComponent["Use " + name + texIStr].as<bool>();
                            auto path = meshComponent["Path " + name + texIStr].as<std::string>();
                            if (path != "")
                                textures[texIdx].Component = Renderer::GetTexture(path);
                            else
                                textures[texIdx].Component = nullptr;
                            
                            if (textures[texIdx].Component) {
                                IK_CORE_INFO("                        Texture Use: {0}", textures[texIdx].Use);
                                IK_CORE_INFO("                        Texture Path: {0}", textures[texIdx].Component->GetfilePath());
                            }
                        }
                        mc.Mesh->AddMaterial(name, prop, textures, invertX, invertY);
                    }
                }
            }
        }
    }
    return true;
}
