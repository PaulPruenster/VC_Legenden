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
        float numberOfLights = 5.0;
        // Normalize the cross product of the X and Y derivatives
        vec3 dFdx_position = dFdx(tFragPos);
        vec3 dFdy_position = dFdy(tFragPos);

        vec3 a = vec3(1.0, 0.0, dFdx_position.x);
        vec3 b = vec3(0.0, 1.0, dFdy_position.x);

        vec3 normal = normalize(cross(a, b));
        normal = tNormal;
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



        vec3 light_spec = vec3(0.0, 0.0, 0.0);

        for (int i = 0; i < numberOfLights; i++)
        {
            vec3 postion_diff = uPointLights[i].position - tFragPos;
            float position_norm = length(postion_diff);
            float intensity = 1.0 / position_norm * position_norm;
            intensity = 2.0;
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
