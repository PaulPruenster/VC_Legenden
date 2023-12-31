#include "water.h"
#include "mygl/geometry.h"
#include "math.h"

Water waterCreate(const Vector4D &color)
{
    Water water;

    water.vertices.resize(grid::vertexPos.size());
    Vector4D colorAdjust(0.0, 0.0, 0.0, 0.0);
    for (unsigned i = 0; i < water.vertices.size(); i++)
    {
        water.vertices[i] = {grid::vertexPos[i] + 0.1, color + colorAdjust};
        colorAdjust += Vector4D(0.01, 0.01, 0.05, 0.0);
        if (i % 6 == 0)
        {
            colorAdjust = {0.0, 0.0, 0.0, 0.0};
        }
    }
    water.mesh = meshCreate(water.vertices, grid::indices, GL_DYNAMIC_DRAW, GL_STATIC_DRAW);
    return water;
}

void waterUpdate(Water &water, float time)
{
    WaveParams *params = WaterSim().parameter;
    for (unsigned i = 0; i < water.vertices.size(); i++)
    {
        // Sum the displacements from all the waves.
        float displacement = 0.0f;
        for (int j = 0; j < 3; j++)
        {
            // Compute the displacement from this wave.
            WaveParams p = params[j];
            float waveDisplacement = p.amplitude * sin(p.omega * (water.vertices[i].pos.x * p.direction.x + water.vertices[i].pos.z * p.direction.y) + time * p.phi);

            // Add the displacement from this wave to the total displacement.
            displacement += waveDisplacement;
        }
        water.vertices[i].pos.y = displacement;
    }
    meshDelete(water.mesh);
    water.mesh = meshCreate(water.vertices, grid::indices, GL_DYNAMIC_DRAW, GL_STATIC_DRAW);
}

float calculateHeightAtPosition(float time, float x, float z)
{
    WaveParams *params = WaterSim().parameter;

    float displacement = 0.0f;
    for (int j = 0; j < sizeof(params) / sizeof(WaveParams *); j++)
    {
        // Compute the displacement from this wave.
        WaveParams p = params[j];
        float waveDisplacement = p.amplitude * sin(p.omega * (x * p.direction.x + z * p.direction.y) + time * p.phi);

        // Add the displacement from this wave to the total displacement.
        displacement += waveDisplacement;
    }
    return displacement;
}

void waterDelete(Water &water) { meshDelete(water.mesh); }
