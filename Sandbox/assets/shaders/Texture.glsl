// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color; // Not needed for 3D
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex; // Not needed for 3D
layout(location = 4) in float a_TilingFactor; // Not needed for 3D

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
    // Ensure the texture index is within bounds
    int TexIndex = int(v_TexIndex);
    if (TexIndex < 0 || TexIndex >= 32) {
        color = vec4(1.0, 0.0, 0.0, 1.0); // Output red color for debugging
        return;
    }

    // Sample the texture
    vec4 texColor = texture(u_Textures[TexIndex], v_TexCoord * v_TilingFactor);

    // Discard if alpha is less than 0.1
    if (texColor.a < 0.1)
        discard;

    // Blend texture color with vertex color
    color = texColor * v_Color;
}
