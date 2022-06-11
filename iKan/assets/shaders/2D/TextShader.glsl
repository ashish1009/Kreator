//
//  TextShader.glsl
//  Kreator
//
//  Created by iKan on 01/06/22.
//

// vertex Shader
#type vertex
#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}

// Fragment Shader
#type fragment
#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}