#type vertex
#version 430 core

layout(location = 0) in vec2 a_Position;
out vec2 v_TexCoord;
out float v_AspectRatio;
out vec2 v_Dimensions;

uniform float u_AspectRatio;
uniform vec2 u_Dimensions;

void main()
{
    gl_Position = vec4(a_Position, 0.0, 1.0);
    v_TexCoord = (a_Position + 1.0) * 0.5; // 0..1 texcoords
    v_AspectRatio = u_AspectRatio;
    v_Dimensions = u_Dimensions;
}

#type fragment
#version 430 core

out vec4 FragColor;
in vec2 v_TexCoord;
in float v_AspectRatio;
in vec2 v_Dimensions;

struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
    float _pad; // padding for std430 alignment
};

struct ray
{
    vec3 origin;
    vec3 direction;
};

layout(std430, binding = 1) buffer Spheres {
    Sphere spheres[];
};

vec3 camPos = vec3(0.0, 0.0, 0.0);
float focalLength = 1.0f;

vec3 CastRay(vec3 o, vec3 d, float t)
{
    //return o + t * d;


}

void main()
{
    vec2 coordinate = gl_FragCoord.xy;

    vec2 pixelDelta = vec2(1.0) / v_Dimensions;

    vec3 blue = vec3(0.44, 0.71, 0.84);
    vec3 white = vec3(0.8, 0.8, 1.0);

    vec3 color = mix(white, blue, v_TexCoord.y);


    FragColor = vec4(color, 1.0);
}
