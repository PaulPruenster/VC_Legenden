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

struct SpotLight
{
    vec3 direction;
    vec3 color;
    vec3 position;
    float angle;
};

out vec4 FragColor;

uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform vec3 cameraPosition;

uniform SpotLight uSpotLights[4];

in vec3 tNormal;
in vec3 tFragPos;

void main(void)
{
    float ambientCoefficient = 1.0;
    float diffuseCoefficient = 1.0;
    float specularCoefficient = 1.0;
    
    vec3 globalAmbient = vec3(0.1, 0.1, 0.1);
    vec3 normal = tNormal;

    vec3 lightDir = normalize(-uDirectionalLight.direction);
    vec3 viewDir = normalize(cameraPosition - tFragPos);
    vec3 halfVec = normalize(lightDir + viewDir);

    // Calculate ambient reflection
    vec3 ambient = ambientCoefficient * uMaterial.ambient * globalAmbient;

    // Calculate diffuse reflection
    float diff = dot(normal, lightDir);
    vec3 diffuse = diffuseCoefficient * uMaterial.diffuse * uDirectionalLight.color * diff;

    // Calculate specular reflection
    float spec = pow(max(dot(normal, halfVec), 0.0), uMaterial.shininess);
    vec3 specular = specularCoefficient * uMaterial.specular * uDirectionalLight.color * spec;

    for (int i = 0; i < 4; i++)
    {
        vec3 lightDirection = uSpotLights[i].position - tFragPos;
        float lightDistance = length(lightDirection);
        float intensity = 5.0 / (lightDistance * lightDistance);
        lightDirection = normalize(lightDirection);

        // if fragment is outside of the light cone, we can stop
        if (acos(dot(uSpotLights[i].direction, lightDirection)) > uSpotLights[i].angle) {
            continue;
        }

        // Calculate the half vector
        halfVec = normalize(lightDirection + viewDir);

        // Calculate diffuse reflection
        diff = dot(normal, lightDirection);
        diffuse += diffuseCoefficient * uMaterial.diffuse * uSpotLights[i].color * diff * intensity;

        // Calculate specular reflection
        spec = pow(max(dot(normal, halfVec), 0.0), uMaterial.shininess);
        specular += specularCoefficient * uMaterial.specular * uSpotLights[i].color * spec;
    }

    // Combine diffuse and specular components
    vec3 result = ambient + diffuse + specular;
    result = clamp(result, 0.0, 1.0);
    FragColor = vec4(result, 1.0);
}
