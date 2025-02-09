#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position; // Triangle vertex positions
layout(location = 1) in vec2 a_TexCoord; // Texture coordinates

out vec3 v_WorldPosition; // Pass world positions to fragment shader
out vec2 v_TexCoord;      // Pass texture coordinates to fragment shader

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    v_WorldPosition = vec3(u_Model * vec4(a_Position, 1.0)); // Transform to world space
    v_TexCoord = a_TexCoord; // Pass texture coordinates
    gl_Position = u_ViewProjection * vec4(v_WorldPosition, 1.0); // Final position in clip space
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_WorldPosition; // Vertex position in world space
in vec2 v_TexCoord;      // Texture coordinates

uniform vec3 u_CameraPosition;  // Camera origin in world space
uniform vec3 u_LightPosition;   // Light source position
uniform vec3 u_BackgroundColor; // Background colour

const vec3 hardcodedTrianglePositions[3] = vec3[3](
    vec3(-1.0, -1.0, -3.0), // Triangle 1 Vertex 1
    vec3(1.0, -1.0, -3.0),  // Triangle 1 Vertex 2
    vec3(0.0, 1.0, -3.0)   // Triangle 1 Vertex 3
    );

const vec3 hardcodedTriangleColors[3] = vec3[3](
    vec3(1.0, 0.0, 0.0), // Red
    vec3(0.0, 1.0, 0.0), // Green
    vec3(0.0, 0.0, 1.0)  // Blue
    );

// Function to compute ray-triangle intersection using the Möller-Trumbore algorithm
bool RayTriangleIntersect(vec3 rayOrigin, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, out float t, out vec3 hitNormal)
{
    const float EPSILON = 1e-8;
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

    vec3 h = cross(rayDir, edge2);
    float a = dot(edge1, h);
    if (abs(a) < EPSILON) return false; // Ray is parallel to the triangle

    float f = 1.0 / a;
    vec3 s = rayOrigin - v0;
    float u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) return false;

    vec3 q = cross(s, edge1);
    float v = f * dot(rayDir, q);
    if (v < 0.0 || u + v > 1.0) return false;

    t = f * dot(edge2, q);
    if (t > EPSILON) { // Ray intersects the triangle
        hitNormal = normalize(cross(edge1, edge2));
        return true;
    }
    return false;
}

void main()
{
    // Normalised screen coordinates
    vec2 uv = (gl_FragCoord.xy / vec2(800.0, 600.0)) * 2.0 - 1.0; // Assuming screen resolution of 800x600
    uv.y *= -1.0; // Flip Y-axis for OpenGL coordinates

    // Define the ray
    vec3 rayOrigin = u_CameraPosition;
    vec3 rayDir = normalize(vec3(uv, -1.0)); // Perspective ray pointing into the scene

    // Check hardcoded triangle
    float closestT = 1e8;   // Track the closest intersection
    vec3 closestNormal;     // Normal at the closest intersection
    vec3 hitColor = u_BackgroundColor; // Default to background colour

    // Test against hardcoded triangle
    float t;
    vec3 hitNormal;
    if (RayTriangleIntersect(rayOrigin, rayDir,
        hardcodedTrianglePositions[0],
        hardcodedTrianglePositions[1],
        hardcodedTrianglePositions[2], t, hitNormal)) {
        if (t < closestT) {
            closestT = t;
            closestNormal = hitNormal;
            hitColor = vec3(v_TexCoord, 1.0); // Use texture coordinates for colour
        }
    }

    // Lighting
    if (closestT < 1e8) {
        vec3 hitPoint = rayOrigin + closestT * rayDir;
        vec3 lightDir = normalize(u_LightPosition - hitPoint);
        float brightness = max(dot(closestNormal, lightDir), 0.0);
        color = vec4(hitColor * brightness, 1.0);
    }
    else {
        color = vec4(u_BackgroundColor, 1.0); // Background colour if no hit
    }
}
