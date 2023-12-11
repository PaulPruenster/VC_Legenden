#version 330 core

struct Material
{
    vec3 diffuse;
    vec3 ambient;
    float shininess; // Shininess factor for specular reflection
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

out vec4 FragColor;

uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform vec3 cameraPosition;
uniform bool isDay;

in vec3 tNormal;
in vec3 tFragPos;

void main(void)
{
    vec3 ambient = 0.3 * uMaterial.ambient;
    vec3 normal = normalize(tNormal);

    vec3 lightDir = normalize(-uDirectionalLight.direction);

    // Calculate the half vector
    vec3 viewDir = normalize(cameraPosition - tFragPos);
    vec3 halfVec = normalize(lightDir + viewDir);

    // Calculate diffuse reflection
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = uMaterial.diffuse * diff;

    // Calculate specular reflection (Blinn-Phong model)
    float spec = pow(max(dot(normal, halfVec), 0.0), uMaterial.shininess);
    vec3 specular = uDirectionalLight.color * spec;

    // Combine diffuse and specular components
    vec3 result = ambient + diffuse + specular;
    if (!isDay) result *= 0.4;
    FragColor = vec4(result, 1.0);
}
