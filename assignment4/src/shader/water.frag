#version 330 core

struct Material
{
    vec3 diffuse;
};

out vec4 FragColor;

uniform Material uMaterial;

in float displacement;
in vec3 varyingNormal;

void main(void)
{
    vec3 color = mix(uMaterial.diffuse, vec3(1.0), displacement) * varyingNormal;
    FragColor = vec4(color, 1.0);
}
