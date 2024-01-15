#include "camera.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

namespace detail
{

glm::vec3 sphericalCoords(const Camera& cam)
{
    glm::vec3 cartVec = cam.position - cam.lookAt;

    auto r = length(cartVec);
    auto phi = atan2(cartVec.x, cartVec.z);
    auto theta = atan2(sqrt(cartVec.x * cartVec.x + cartVec.z * cartVec.z), cartVec.y);

    return glm::vec3(r, phi, theta);
}

}

Camera cameraCreate(float width, float height, float fov, float nearPlane, float farPlane, const glm::vec3 &initPos, const glm::vec3 &lookAt, const glm::vec3 &initUp)
{
    return {width, height, fov, nearPlane, farPlane, initPos, lookAt, initUp};
}

glm::mat4 cameraProjection(const Camera &cam)
{
    return glm::perspective(cam.fov, cam.width/cam.height, cam.nearPlane, cam.farPlane);
}

glm::mat4 cameraView(const Camera &cam)
{
    return glm::lookAt(cam.position, cam.lookAt, cam.initUp);
} 

void cameraUpdateOrbit(Camera& cam, const glm::vec2& mouseDiff, float zoom)
{
    glm::vec3 spherCoord = detail::sphericalCoords(cam);
    float r = spherCoord[0];
    float phi = spherCoord[1];
    float theta = spherCoord[2];

    phi += mouseDiff.x * (M_PI / cam.width);
    theta += mouseDiff.y * (M_PI / cam.height);
    r += zoom * r;

    theta = std::clamp<float>(theta, 1e-4, M_PI - 1e-4);
    r = std::max(r, 1e-4f);

    glm::vec3 cartCoord(r * sin(theta) * sin(phi), r * cos(theta), r * sin(theta) * cos(phi));

    cam.position = cam.lookAt + cartCoord;
}
