// Texture3D.glsl
// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
flat out int v_TexIndex; // flat: no interpolation
out vec3 v_Normal;

void main()
{
    v_TexCoord = a_TexCoord;
    v_TexIndex = int(a_TexIndex);  // Force an integer value
    v_Normal = a_Normal;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;
flat in int v_TexIndex;  // flat matching the vertex shader

uniform sampler2D u_Textures[32];

void main()
{
    // Validate the texture index
    if (v_TexIndex < 0 || v_TexIndex >= 32) {
        color = vec4(0.2, 0.3, 0.7, 1.0); // debug color (blue)
        return;
    }
    vec4 texColor = texture(u_Textures[v_TexIndex], v_TexCoord);
    color = texColor;
}
