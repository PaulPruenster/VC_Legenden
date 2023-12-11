#version 330 core

struct Material
{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess; // Shininess factor for specular reflection
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct DecayingPointLight
{
    vec3 direction;
    vec3 color;
    vec3 position;
};

out vec4 FragColor;

uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform vec3 cameraPosition;
uniform bool isDay;

uniform DecayingPointLight uPointLights[4];

in vec3 tNormal;
in vec3 tFragPos;

void main(void)
{
    float ambientCoefficient = 1.0;
    float diffuseCoefficient = 1.0;
    float specularCoefficient = 1.0;
    vec3 globalAmbient = vec3(0.1, 0.1, 0.1);
    float numberOfLights = 5.0;
    vec3 normal = tNormal;
    vec3 ambient = ambientCoefficient * uMaterial.ambient * globalAmbient;

    vec3 lightDir = normalize(-uDirectionalLight.direction);

    // Calculate the half vector
    vec3 viewDir = normalize(cameraPosition - tFragPos);
    vec3 halfVec = normalize(lightDir + viewDir);

    // Calculate diffuse reflection
    float diff = dot(normal, lightDir);
    vec3 diffuse = diffuseCoefficient * uMaterial.diffuse * uDirectionalLight.color * diff;

    // Calculate specular reflection (Blinn-Phong model)
    float spec = pow(max(dot(normal, halfVec), 0.0), uMaterial.shininess);
    vec3 specular = specularCoefficient * uMaterial.specular * uDirectionalLight.color * spec;


    vec3 light_spec = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < numberOfLights; i++)
    {
        vec3 position_diff = uPointLights[i].position - tFragPos;
        float position_norm = length(position_diff);
        float intensity = 1.0 / (position_norm * position_norm);
        // Calculate the half vector
        vec3 lightDir = normalize(uPointLights[i].position - tFragPos);
        vec3 halfVec = normalize(lightDir + viewDir);

        float spec = pow(max(dot(normal, halfVec), 0.0), uMaterial.shininess);
        vec3 specular_point = uPointLights[i].color * spec;
        light_spec += (specular_point * intensity);
    }
    specular += light_spec;
    specular /= numberOfLights;

    // Combine diffuse and specular components
    vec3 result = ambient + diffuse + specular;
    if (!isDay) result *= 0.4;
    FragColor = vec4(result, 1.0);
}
