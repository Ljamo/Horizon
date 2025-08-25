#type vertex
#version 430 core

layout(location = 0) in vec2 a_Position;
out vec2 v_TexCoord;

void main()
{
    gl_Position = vec4(a_Position, 0.0, 1.0);
    v_TexCoord = (a_Position + 1.0) * 0.5; // 0..1 texcoords
}

#type fragment
#version 430 core

out vec4 FragColor;
in vec2 v_TexCoord;

struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
    float _pad; // padding for std430 alignment
};

layout(std430, binding = 1) buffer Spheres {
    Sphere spheres[];
};

vec3 camPos = vec3(0.0, 0.0, 0.0);

// ray–sphere intersection
bool intersectSphere(vec3 ro, vec3 rd, Sphere s, out float t)
{
    vec3 oc = ro - s.center;
    float b = dot(oc, rd);
    float c = dot(oc, oc) - s.radius * s.radius;
    float h = b * b - c;
    if (h < 0.0) return false;
    h = sqrt(h);
    t = -b - h;
    if (t < 0.0) t = -b + h;
    return t > 0.0;
}

void main()
{
    // generate ray
    vec2 ndc = v_TexCoord * 2.0 - 1.0;
    vec3 ro = camPos;
    vec3 rd = normalize(vec3(ndc, -1.0));

    float tClosest = 1e20;
    vec3 finalColor = vec3(0.0);

    for (int i = 0; i < spheres.length(); i++)
    {
        float t;
        if (intersectSphere(ro, rd, spheres[i], t) && t < tClosest)
        {
            tClosest = t;
            vec3 hitPos = ro + rd * t;
            vec3 n = normalize(hitPos - spheres[i].center);
            float diff = max(dot(n, normalize(vec3(1.0, 1.0, -1.0))), 0.0);
            finalColor = spheres[i].color * diff;
        }
    }

    FragColor = vec4(finalColor, 1.0);
}
