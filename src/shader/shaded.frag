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

vec3 CalcDirLight(DirLight light);


void main()
{
    vec3 result = CalcDirLight(dirLight);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light)
{
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    vec3 ambient = light.ambient * material.ambient;

    vec3 lightDirection = normalize(-light.direction);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    //     return (ambient + diffuse + specular);
    return (ambient + diffuse);
}
