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

uniform mat4 u_InverseView;
uniform mat4 u_InverseProj;
uniform float u_Time;

out vec4 FragColor;
in vec2 v_TexCoord;
in float v_AspectRatio;
in vec2 v_Dimensions;
mat4 v_InverseView = u_InverseView;
mat4 v_InverseProj = u_InverseProj;

const float INF = 3.402823466e+38;
vec3 sunDir = normalize(vec3(-1, -1, -1));
const float sampleCount = 4;
vec2 u_coords = vec2(0);
vec2 pixelDelta = vec2(1 / v_Dimensions.x, 1 / v_Dimensions.y);
int numBounces = 4;

struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
    float roughness; // padding for std430 alignment
};

layout(std430, binding = 1) buffer Spheres {
    Sphere spheres[];
};

int sphereCount = spheres.length();

struct Ray
{
    vec3 origin;
    vec3 direction;
    // float distance;
};

struct HitRecord
{
    //vec3 position;
    //vec3 normal;
    vec3 pixelColor;
    int sphereNum;
    vec3 lastPixelColor;
    float t;
};

HitRecord hitRecord;

float CastRay(Ray ray, int sphereIndex)
{

    // a + bt
    // 
    //      A                   B                   C
    // (bx^2 + by^2)t^2 + (2(ax*bx + ay*by))t + (ax^2 + ay^2 - r^2) = 0

    // a = ray origin
    // b = ray direction
    // r = radius of sphere
    // t = hit distance

    float radius = spheres[sphereIndex].radius;
    vec3 center = spheres[sphereIndex].center;

    vec3 oc = center - ray.origin;

    float a = dot(ray.direction, ray.direction);
    // Equivalent to dot product:
    // (ray.origin.x * ray.direction.x + ray.origin.y * ray.direction.y + ray.origin.z * ray.direction.z);
    float b = -2 * dot(ray.direction, oc);
    float c = dot(oc, oc) - radius * radius;

    float d = b * b - 4 * a * c;

    if (d < 0)
    {
        return INF;
    }

    float sd = sqrt(d);
    float t = (-b - sd) / (a * 2);  //min((-b - sd) / (a * 2), (-b + sd) / (a * 2));

    if (t < 0)
        return INF;

    return t;
}

vec3 at(Ray ray, float t)
{
    return ray.origin + ray.direction * t;
}

vec3 CalculateNormals(Ray ray, float t, vec3 sphereCenter)
{
    return normalize(at(ray, t) - sphereCenter);
}

float RandFromVec2(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float RandFromFloat(float seed)
{
    return sin(fract(sin(seed) * 43758.5453123));

}

vec3 SampleSquare()
{
    float rand = RandFromFloat(u_Time);
    return vec3(rand - 0.5, RandFromFloat(rand) - 0.5, 0);
}

bool HasHit(Ray ray, int sphereIndex)
{
    vec3 offset = SampleSquare();
    // Working here
    vec2 s = u_coords + offset.xy * pixelDelta;

    float radius = spheres[sphereIndex].radius;
    vec3 center = spheres[sphereIndex].center;

    // vec3 dir = s - ray.origin;
    vec3 oc = center - ray.origin;


    float a = dot(ray.direction, ray.direction);
    // Equivalent to dot product:
    // (ray.origin.x * ray.direction.x + ray.origin.y * ray.direction.y + ray.origin.z * ray.direction.z);
    float b = -2 * dot(ray.direction, oc);
    float c = dot(oc, oc) - radius * radius;

    float d = b * b - 4 * a * c;

    if (d < 0)
    {
        return false;
    }

    return true;
}

vec3 Reflect(vec3 I, vec3 N)
{
    return normalize(I - 2.0 * dot(I, N) * N);
}

// const int NUM_SUNS = 3;
// vec3 sunDirs[NUM_SUNS] = vec3[](
//     normalize(vec3(-1, -1, -1)),   // sun 1
//     normalize(vec3(1, -0.5, -0.5)),// sun 2
//     normalize(vec3(0, -1, -0.2))   // sun 3
//     );
// 
// float CalculateLightIntensity(vec3 n)
// {
//     float intensity = 0.0;
//     for (int i = 0; i < NUM_SUNS; i++) {
//         intensity += max(dot(n, -sunDirs[i]), 0.0);
//     }
//     return clamp(intensity, 0.0, 1.0); // prevents over-brightening
// }

float CalculateLightIntensity(vec3 n)
{
    return max(dot(n, -sunDir), 0.0);
}

vec3 GenColor(vec2 coords)
{
    u_coords = coords;

    Ray ray;
    // ray.direction = vec3(coords.x, coords.y, -1.0);
    //ray.direction = normalize(ray.direction);
    vec4 target = v_InverseProj * vec4(coords.x, coords.y, 1, 1);
    ray.direction = vec3(v_InverseView * vec4(normalize(vec3(target) / target.w), 0)); // world space

    hitRecord.t = INF;
    hitRecord.sphereNum = 0;

    ray.origin = vec3(u_InverseView[3]);

    vec2 tCoords = coords * u_Time;

    float rand = RandFromVec2(tCoords);
    float rand2 = RandFromFloat(rand);
    float rand3 = RandFromFloat(rand2);

    hitRecord.pixelColor = vec3(1.0);

    float multiplier = 1.0f;

    // Calculate rays
    for (int j = 0; j < numBounces; j++)
    {
        bool didHit = false;
        vec3 tColor = vec3(1.0);

        for (int i = 0; i < sphereCount; i++)
        {
            // Min dist
            float t = CastRay(ray, i);
            if (hitRecord.t <= t)
                continue;

            hitRecord.t = t;
            tColor = spheres[i].color;           
            hitRecord.sphereNum = i;

            didHit = true;
        }

        if (didHit == false)
        {
            float a = 0.5 * (ray.direction.y + 1.0);
            hitRecord.pixelColor *= (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
            break;
        }

        // Calculate new color and hit position
        vec3 normal = CalculateNormals(ray, hitRecord.t, spheres[hitRecord.sphereNum].center);
        // normal += RandFromVec2(tCoords) * spheres[hitRecord.sphereNum].roughness;
        vec3 col = tColor * CalculateLightIntensity(normal);
        hitRecord.pixelColor = col * multiplier;

        multiplier *= 0.7f;

        ray.origin = at(ray, hitRecord.t);
        vec3 randVec = normalize(vec3(
            RandFromVec2(tCoords * 1) - 0.5,
            RandFromVec2(tCoords * 2) - 0.5,
            RandFromVec2(tCoords * 3) - 0.5
        ));

        vec3 tNormal = normalize(normal + spheres[hitRecord.sphereNum].roughness * randVec);
        ray.direction = Reflect(ray.direction, tNormal);

        hitRecord.t = INF;
    }

    
    return hitRecord.pixelColor;
}

void main()
{
    vec2 coordinates = gl_FragCoord.xy / v_Dimensions;
    coordinates = coordinates * 2 - 1.0;

    FragColor = vec4(GenColor(coordinates), 1.0);
}
