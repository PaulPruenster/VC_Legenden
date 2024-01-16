#version 330 core

in vec4 tColor;
in vec2 tUV;
out vec4 FragColor;
uniform sampler2D textureMap;

void main(void)
{
    FragColor = vec4(texture(textureMap, tUV).rgb , 1.0);
}
