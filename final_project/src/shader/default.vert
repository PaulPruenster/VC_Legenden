#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec4 tColor;
out vec3 tNormal;
out vec2 tUV;

void main(void)
{
    gl_Position = uProj * uView * uModel * vec4(aPosition, 1.0);
    tColor = aColor;
    tNormal = aNormal;
    tUV = aUV;
}
