#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec4 light_color;

void main()
{   
    FragColor = texture(skybox, TexCoords);
    FragColor *= light_color;
}