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

// vertex normal
in vec3 Normal;
// vertex position
in vec3 FragPos;
// vertex Seg result
flat in int Seg;
// vertex TexCoords
in vec2 TexCoords;

out vec4 FragColor;

// camera position
uniform vec3 viewPos;

// object material
uniform Material material;

// direction light
uniform DirLight dirLight;

// max seg class
uniform int maxSegClass;

// all colors
const int numColors = 100;
uniform vec3 allColors[numColors];

vec3 CalcDirLight(DirLight light);

float ColorInterpolation(float target, float cMin, float cMax);


void main()
{
    vec3 result = CalcDirLight(dirLight);

    if (maxSegClass != 0)
    {
//         result *= ColorInterpolation(float(Seg) / float(maxSegClass), 0.4, 1);
        result *= allColors[Seg];
    }

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

float ColorInterpolation(float target, float cMin, float cMax)
{
    target = clamp(target, 0, 1);
    return cMin + (target) * (cMax - cMin);
}
