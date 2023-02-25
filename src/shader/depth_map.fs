#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;

uniform DirLight dirLight;

uniform float zMin;
uniform float zMax;

uniform float grayMin;
uniform float grayMax;

vec3 DepthMap(DirLight dirLight, vec3 FragPos);


void main()
{
    vec3 result = DepthMap(dirLight, FragPos);

    FragColor = vec4(result, 1.0);
}

vec3 DepthMap(DirLight dirLight, vec3 FragPos)
{
    float gray = 1 - (FragPos.z - zMin) / (zMax - zMin);
    gray = clamp(gray, grayMin, grayMax);
    gray = grayMin + gray * (grayMax - grayMin);
    return gray * dirLight.specular * material.specular;
}
