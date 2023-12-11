#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

struct WaveParams
{
    float amplitude;
    float phi;
    float omega;
    vec2 direction;
};

struct WaterSim
{
    WaveParams parameter[3];
    float accumTime;
};

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform WaterSim uWaterSim;

out vec3 tNormal;
out vec3 tFragPos;
out vec2 tUV;


void main(void)
{
    float time = uWaterSim.accumTime;

    float height = 0.0;
    float dfdx = 0.0;
    float dfdz = 0.0;
    for (int i = 0; i < 3; i++) {
        WaveParams p = uWaterSim.parameter[i];
        height += p.amplitude * sin(dot(normalize(p.direction), aPosition.xz) * p.omega + time * p.phi);
        dfdx += p.amplitude * cos(dot(normalize(p.direction), aPosition.xz) * p.omega + time * p.phi) * p.direction.x * p.omega;
        dfdz += p.amplitude * cos(dot(normalize(p.direction), aPosition.xz) * p.omega + time * p.phi) * p.direction.y * p.omega;
    }

    vec3 transformedPosition = aPosition;
    transformedPosition.y += height;
    gl_Position = uProj * uView * uModel * vec4(transformedPosition, 1.0);
    tFragPos = vec3(uModel * vec4(transformedPosition, 1.0));

    vec3 x_normal = normalize(vec3(1.0, dfdx, 0.0));
    vec3 z_normal = normalize(vec3(0.0, dfdz, 1.0));
    tNormal = normalize(cross(z_normal, x_normal));

    tUV = aUV;
}
