#pragma once

#include <glm/glm.hpp>

struct Camera
{
    float width;
    float height;
    float fov;
    float nearPlane;
    float farPlane;
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 initUp;
};

/**
 * @brief Function to initialize a camera.
 *
 * @param width Image width.
 * @param height Image height.
 * @param fov Field of view (in rad).
 * @param nearPlane Plane defining closest points that get rendered.
 * @param farPlane Plane defining fardest points that get rendered.
 * @param initPos Initial position of camera.
 * @param lookAt Point at which the camera is looking at.
 * @param initUp Initial up direction of camera.
 *
 * @return Initialized camera object.
 */
Camera cameraCreate(float width, float height, float fov, float nearPlane, float farPlane, const glm::vec3& initPos, const glm::vec3& lookAt = {0, 0, 0}, const glm::vec3& initUp = {0, 1, 0});

/**
 * @brief Get projection matrix from a camera.
 *
 * @param cam Camera from which the projection matrix is calculated.
 *
 * @return Projection matrix.
 */
glm::mat4 cameraProjection(const Camera& cam);

/**
 * @brief Get view matrix from a camera.
 *
 * @param cam Camera from which the view matrix is calculated.
 *
 * @return View matrix.
 */
glm::mat4 cameraView(const Camera& cam);

/**
 * @brief Update camera position on the orbit around the look at point using spherical coordinates.
 *
 * @param cam Camera that gets updated.
 * @param mouseDiff X and Y distances that are used to change the spherical coordinate angles of the camera position.
 * with respect to the look at point.
 * @param zoom Factor to zoom in (-) or out (+) (distance of camera position to look at point is de-/increased).
 */
void cameraUpdateOrbit(Camera &cam, const glm::vec2 &mouseDiff, float zoom);
