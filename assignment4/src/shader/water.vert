#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform float time;

out vec3 tNormal;
out vec3 tFragPos;
out vec2 tUV;

out float displacement;

void main(void)
{
    float wave1 = 0.6 * sin(0.25 * (aPosition.x * 1.0 + aPosition.z * 1.0) + time * 0.5);
    float wave2 = 0.7 * sin(0.1 * (aPosition.x * 1.0 + aPosition.z * -1.0) + time * 0.25);
    float wave3 = 0.1 * sin(0.9 * (aPosition.x * -1.0 + aPosition.z * 1.0) + time * 0.9);

    vec3 transformedPosition = aPosition + wave1 + wave2 + wave3;
    displacement = wave1 + wave2 + wave3;

    gl_Position = uProj * uView * uModel * vec4(transformedPosition, 1.0);
    tFragPos = vec3(uModel * vec4(aPosition, 1.0));
    tNormal = mat3(transpose(inverse(uModel))) * aNormal;
    tUV = aUV;
}
