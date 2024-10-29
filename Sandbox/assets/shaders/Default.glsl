// Default 3d Shader

// Vertex Shader
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_Projection;	
uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}