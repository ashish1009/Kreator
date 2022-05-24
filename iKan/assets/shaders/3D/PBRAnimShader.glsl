//
//  PBRShader.glsl
//  Kreator
//
//  Created by iKan on 24/05/22.
//

// vertex Shader
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;

layout(location = 5) in ivec4 a_BoneIndices;
layout(location = 6) in vec4 a_BoneWeights;

layout(location = 7) in int a_ObjectID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

const int MAX_BONES = 100;
uniform mat4 u_BoneTransforms[100];

out VertexOutput
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TexCoord;
    mat3 WorldNormals;
    vec3 Binormal;
    
    float ObjectID;
} vs_Output;

/// Vertex Main Function
void main()
{
    mat4 boneTransform = u_BoneTransforms[a_BoneIndices[0]] * a_BoneWeights[0];
    boneTransform += u_BoneTransforms[a_BoneIndices[1]] * a_BoneWeights[1];
    boneTransform += u_BoneTransforms[a_BoneIndices[2]] * a_BoneWeights[2];
    boneTransform += u_BoneTransforms[a_BoneIndices[3]] * a_BoneWeights[3];
    
    vec4 localPosition = boneTransform * vec4(a_Position, 1.0);
    
    vs_Output.WorldPosition = vec3(u_Transform * boneTransform * vec4(a_Position, 1.0));
    vs_Output.Normal = mat3(u_Transform) * mat3(boneTransform) * a_Normal;
    vs_Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
    vs_Output.WorldNormals = mat3(u_Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
    vs_Output.Binormal = mat3(boneTransform) * a_Binormal;
    
    vs_Output.ObjectID  = a_ObjectID;
    
    gl_Position = u_ViewProjection * u_Transform * localPosition;
}

// Fragment Shader
#type fragment

#version 330 core
layout (location = 0) out vec4 o_Color;
layout (location = 1) out int  o_IDBuffer;

in VertexOutput
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TexCoord;
    mat3 WorldNormals;
    vec3 Binormal;
    
    float ObjectID;
} vs_Input;

struct Material
{
    vec3 AlbedoColor;
    
    float Metalness;
    float Roughness;
    float TilinghFactor;
};

// Texture Uniforms
uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_MetallicTexture;
uniform sampler2D u_SpecularTexture;

// Texture Toggle
uniform float u_AlbedoTextureToggle;
uniform float u_NormalTextureToggle;
uniform float u_RoughnessTextureToggle;
uniform float u_MetallicTextureToggle;
uniform float u_SpecularTextureToggle;

// Texture Invert
uniform float u_TextureInvertVertical;
uniform float u_TextureInvertHorizontal;

uniform Material u_Material;
uniform vec3 u_CameraPosition;

struct PBRParameters
{
    vec3 Albedo;
    vec3 Specular;
    vec3 Normal;
    
    vec3 View;
    float NdotV;
    
    float Metalness;
    float Roughness;
};
PBRParameters m_Params;

/// Fragment Main Function
void main()
{
    float normalizedTexCoordsY = (u_TextureInvertVertical > 0.5) ? 1.0f - vs_Input.TexCoord.y : vs_Input.TexCoord.y;
    float normalizedTexCoordsX = (u_TextureInvertHorizontal > 0.5) ? 1.0f - vs_Input.TexCoord.x : vs_Input.TexCoord.x;
    vec2 normalizedTexCoords = vec2(normalizedTexCoordsX, normalizedTexCoordsY);
    
    m_Params.Albedo    = (u_AlbedoTextureToggle > 0.5) ? texture(u_AlbedoTexture, normalizedTexCoords * u_Material.TilinghFactor).rgb : u_Material.AlbedoColor;
    m_Params.Metalness = (u_MetallicTextureToggle > 0.5) ? texture(u_MetallicTexture, normalizedTexCoords * u_Material.TilinghFactor).r : u_Material.Metalness;
    m_Params.Roughness = (u_RoughnessTextureToggle > 0.5) ? texture(u_RoughnessTexture, normalizedTexCoords * u_Material.TilinghFactor).r : u_Material.Roughness;
    m_Params.Roughness = max(m_Params.Roughness, 0.05); // Minimum roughness of 0.05 to keep specular highlight
    
    m_Params.Specular    = (u_SpecularTextureToggle > 0.5) ? texture(u_SpecularTexture, normalizedTexCoords * u_Material.TilinghFactor).rgb : u_Material.AlbedoColor;
    
    // Normals (either from vertex or map)
    m_Params.Normal = normalize(vs_Input.Normal);
    if (u_NormalTextureToggle > 0.5)
    {
        m_Params.Normal = normalize(2.0 * texture(u_NormalTexture, normalizedTexCoords * u_Material.TilinghFactor).rgb - 1.0f);
        m_Params.Normal = normalize(vs_Input.WorldNormals * m_Params.Normal);
    }
    
    m_Params.View = normalize(u_CameraPosition - vs_Input.WorldPosition);
    m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0f);

    o_Color = vec4(m_Params.Albedo, 1.0f);
    o_IDBuffer = int(vs_Input.ObjectID);
}
