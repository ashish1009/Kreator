//
//  SceneSerialiser.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "SceneSerialiser.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Component.hpp"
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

/// SceneSerializer constructor
SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene) : m_Scene(scene) {
    IK_CORE_INFO("Serialising a scene");
}

/// SceneSerializer destructor
SceneSerializer::~SceneSerializer() {
    IK_CORE_INFO("Scene Serialised");
}

/// Serialize the Scene each enityt
void SceneSerializer::Serialize(const std::string& filepath) {
}

/// Deserialize the Scene Runtime
bool SceneSerializer::Deserialize(const std::string& filepath) {
    return true;
}
