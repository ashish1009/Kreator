//
//  PBRShader.glsl
//  Kreator
//
//  Created by iKan on 25/05/22.
//

// vertex Shader
#type vertex

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Binormal;
layout (location = 5) in int  a_ObjectID;

out VS_OUT
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TexCoord;
    mat3 WorldNormals;
    mat3 WorldTransform;
    vec3 Binormal;
    float ObjectID;
} vs_Output;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

/// Vertex Main Function
void main()
{
    vs_Output.WorldPosition = vec3(u_Transform * vec4(a_Position, 1.0));
    vs_Output.Normal = mat3(u_Transform) * a_Normal;
    vs_Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
    vs_Output.WorldNormals = mat3(u_Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
    vs_Output.WorldTransform = mat3(u_Transform);
    vs_Output.Binormal = a_Binormal;
    vs_Output.ObjectID  = a_ObjectID;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment

#version 330 core
layout (location = 0) out vec4 o_Color;
layout (location = 1) out int  o_IDBuffer;

in VS_OUT
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TexCoord;
    mat3 WorldNormals;
    mat3 WorldTransform;
    vec3 Binormal;
    float ObjectID;
} vs_Input;

// Stores the Material Property
struct Material
{
    vec3 AlbedoColor;
    
    float Metalness;
    float Roughness;
    float TilinghFactor;
};

// Stores the Light Property
struct Light
{
    bool Present;
    
    vec3 Position;
    vec3 Radiance;
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

uniform Light u_Light[4];
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

/// Add Light to Material
vec3 Lighting(vec3 F0)
{
    vec3 result = vec3(0.0);
    
    // Process each light
    for (int i = 0; i < 4; i++)
    {
        Light light = u_Light[i];
        if (light.Present)
        {
            result = vec3(1.0f);
            return result;
        }
    }
    
    return result;
}

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
    
    // calculate reflectance at normal incidence
    // By pre-computing F0 for both dielectrics and conductors we can use the same Fresnel-Schlick approximation for both types of surfaces,
    // but we do have to tint the base reflectivity if we have a metallic surface. We generally accomplish this as follows:
    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, m_Params.Albedo, m_Params.Metalness);

    vec3 lightContribution = Lighting(F0);

    vec3 color = lightContribution;

    // TODO: Add HDR or Gamma correction
    o_Color = vec4(color, 1.0f);
    o_IDBuffer = int(vs_Input.ObjectID);
}
