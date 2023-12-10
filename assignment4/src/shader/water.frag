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

in vec3 tFragPos;
in vec3 tNormal;

void main(void)
{
    // Normalize the cross product of the X and Y derivatives
    vec3 dFdx_position = dFdx(tFragPos);
    vec3 dFdy_position = dFdy(tFragPos);
    vec3 normal = normalize(cross(dFdx_position, dFdy_position));
    vec3 ambient = 0.3 * uMaterial.ambient;

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
    FragColor = vec4(result, 1.0);
}
