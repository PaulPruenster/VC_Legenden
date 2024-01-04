#version 330 core

// struct Material
// {
//     vec3 diffuse;
// };

in vec3 tNormal;
in vec3 tFragPos;
in vec2 tUV;

out vec4 fragColor;

// uniform Material uMaterial;
uniform sampler2D waterTexture;

void main(void)
{
    // fragColor = vec4(uMaterial.diffuse, 1.0);
    fragColor = texture(waterTexture, tUV);
}