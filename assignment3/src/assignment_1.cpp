#include <cstdlib>
#include <iostream>
#include "mygl/shader.h"
#include "mygl/mesh.h"
#include "mygl/geometry.h"
#include "mygl/camera.h"
#include "water.h"

#define NUM_CUBES 7

/* translation and color for the water plane */
namespace waterPlane
{
    const Vector4D color = {0.0f, 0.0f, 0.35f, 1.0f};
    const Matrix4D trans = Matrix4D::identity();
}

/* structure for cube */
struct Cube
{
    const Matrix4D scale;
    const Matrix4D trans;
    const Vector4D color;
};

Cube boat[NUM_CUBES] = {{Matrix4D::scale(1.25f, 0.9f, 3.5f), Matrix4D::translation({0.0f, 0.0f, 0.0f}), {91.0 / 255.0, 58.0 / 255.0, 41.0 / 255.0}},
                        {Matrix4D::scale(0.375f, 0.75f, 0.625f), Matrix4D::translation({0.0f, 1.65f, -1.5f}), {200.0 / 255.0, 200.0 / 255.0, 200.0 / 255.0}},
                        {Matrix4D::scale(0.15f, 1.5f, 0.15f), Matrix4D::translation({0.0f, 2.4f, 1.0f}), {44.0 / 255.0, 28.0 / 255.0, 20.0 / 255.0}},
                        {Matrix4D::scale(1.25f, 0.3f, 0.15f), Matrix4D::translation({0.0f, 1.2f, 3.35f}), {156.0 / 255.0, 107.0 / 255.0, 48.0 / 255.0}},
                        {Matrix4D::scale(1.25f, 0.3f, 0.15f), Matrix4D::translation({0.0f, 1.2f, -3.35f}), {156.0 / 255.0, 107.0 / 255.0, 48.0 / 255.0}},
                        {Matrix4D::scale(0.15f, 0.3f, 3.2f), Matrix4D::translation({1.1f, 1.2f, 0.0f}), {156.0 / 255.0, 107.0 / 255.0, 48.0 / 255.0}},
                        {Matrix4D::scale(0.15f, 0.3f, 3.2f), Matrix4D::translation({-1.1f, 1.2f, 0.0f}), {156.0 / 255.0, 107.0 / 255.0, 48.0 / 255.0}}};

/* struct holding all necessary state variables for scene */
struct
{
    /* camera */
    Camera camera;
    float zoomSpeedMultiplier;
    int cameraMode; /* 1 -> initial camera mode, 2 -> Third person camera */

    /* water */
    WaterSim waterSim;
    Water water;
    Matrix4D waterModelMatrix;

    /* cube mesh and transformations */
    Mesh cubeMesh[NUM_CUBES];
    Matrix4D cubeScalingMatrix[NUM_CUBES];
    Matrix4D cubeTranslationMatrix[NUM_CUBES];
    Matrix4D cubeTransformationMatrix[NUM_CUBES];
    Matrix4D boatTranslationMatrix;
    Matrix4D boatTransformationMatrix;
    float boatTurningRadian;
    float boatVelocity;
    float boatTurningSpeed;
    Vector3D boatFront;

    /* shader */
    ShaderProgram shaderColor;
} sScene;

/* struct holding all state variables for input */
struct
{
    bool mouseLeftButtonPressed = false;
    Vector2D mousePressStart;
    bool buttonPressed[6] = {false, false, false, false, false, false};
} sInput;

/* GLFW callback function for keyboard events */
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    /* called on keyboard event */

    /* close window on escape */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    /* make screenshot and save in work directory */
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        screenshotToPNG("screenshot.png");
    }

    /* input for cube control */
    if (key == GLFW_KEY_W)
    {
        sInput.buttonPressed[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    if (key == GLFW_KEY_S)
    {
        sInput.buttonPressed[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }

    if (key == GLFW_KEY_A)
    {
        sInput.buttonPressed[2] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    if (key == GLFW_KEY_D)
    {
        sInput.buttonPressed[3] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    /* input for camera modes */
    if (key == GLFW_KEY_1)
    {
        sInput.buttonPressed[4] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    if (key == GLFW_KEY_2)
    {
        sInput.buttonPressed[5] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
}

/* GLFW callback function for mouse position events */
void mousePosCallback(GLFWwindow *window, double x, double y)
{
    /* called on cursor position change */
    if (sInput.mouseLeftButtonPressed)
    {
        Vector2D diff = sInput.mousePressStart - Vector2D(x, y);
        cameraUpdateOrbit(sScene.camera, diff, 0.0f);
        sInput.mousePressStart = Vector2D(x, y);
    }
}

/* GLFW callback function for mouse button events */
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        sInput.mouseLeftButtonPressed = (action == GLFW_PRESS || action == GLFW_REPEAT);

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        sInput.mousePressStart = Vector2D(x, y);
    }
}

/* GLFW callback function for mouse scroll events */
void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    cameraUpdateOrbit(sScene.camera, {0, 0}, sScene.zoomSpeedMultiplier * yoffset);
}

/* GLFW callback function for window resize event */
void windowResizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    sScene.camera.width = width;
    sScene.camera.height = height;
}

/* function to setup and initialize the whole scene */
void sceneInit(float width, float height)
{
    /* initialize camera */
    sScene.camera = cameraCreate(width, height, to_radians(45.0f), 0.01f, 500.0f, {10.0f, 14.0f, 10.0f}, {0.0f, 0.0f, 0.0f});
    sScene.zoomSpeedMultiplier = 0.05f;
    sScene.cameraMode = 1;

    /* setup objects in scene and create opengl buffers for meshes */
    for (int i = 0; i < NUM_CUBES; i++)
    {
        sScene.cubeMesh[i] = meshCreate(cube::verticesColored(boat[i].color), cube::indices, GL_STATIC_DRAW, GL_STATIC_DRAW);
        sScene.cubeScalingMatrix[i] = boat[i].scale;
        sScene.cubeTranslationMatrix[i] = boat[i].trans;
        sScene.cubeTransformationMatrix[i] = Matrix4D::identity();
    }
    sScene.boatTranslationMatrix = Matrix4D::translation({0.0f, 0.0f, 0.0f});
    sScene.boatFront = Vector3D(0.0f, 0.0f, 1.0f);
    sScene.boatTransformationMatrix = Matrix4D::rotationY(0);
    sScene.boatTurningRadian = 0.0f;
    sScene.water = waterCreate(waterPlane::color);

    /* setup transformation matrices for objects */
    sScene.waterModelMatrix = waterPlane::trans;

    sScene.boatVelocity = 5.0f;
    sScene.boatTurningSpeed = 0.01f;

    /* load shader from file */
    sScene.shaderColor = shaderLoad("shader/default.vert", "shader/default.frag");
}

/* function to move and update objects in scene (e.g., rotate cube according to user input) */
void sceneUpdate(float dt, float t)
{
    // Water stuff
    waterUpdate(sScene.water, t);

    float distance = 0.7f;
    float y_offset = 0.1f;

    Vector3D oldBoatPosition = sScene.boatTranslationMatrix[3];
    float newBoatY = calculateHeightAtPosition(t, sScene.boatTranslationMatrix[3].x, sScene.boatTranslationMatrix[3].z);
    sScene.boatTranslationMatrix[3].y = newBoatY - y_offset;

    Vector3D PointA;
    Vector3D PointB;
    Vector3D PointC;
    PointA.x = sScene.boatTranslationMatrix[3].x + (distance * cos(0));
    PointA.z = sScene.boatTranslationMatrix[3].z + (distance * sin(0));
    PointA.y = calculateHeightAtPosition(t, PointA.x, PointA.z);
    PointB.x = sScene.boatTranslationMatrix[3].x + (distance * cos(2 * M_PI / 3));
    PointB.z = sScene.boatTranslationMatrix[3].z + (distance * sin(2 * M_PI / 3));
    PointB.y = calculateHeightAtPosition(t, PointB.x, PointB.z);
    PointC.x = sScene.boatTranslationMatrix[3].x + (distance * cos(4 * M_PI / 3));
    PointC.z = sScene.boatTranslationMatrix[3].z + (distance * sin(4 * M_PI / 3));
    PointC.y = calculateHeightAtPosition(t, PointC.x, PointC.z);

    // Calculate tringle angle on z and x achse
    float angleZ = atan2(PointB.y - PointA.y, PointB.x - PointA.x);
    float angleX = atan2(PointC.y - PointA.y, PointC.z - PointA.z);

    // set boat rotation to the angle
    sScene.boatTransformationMatrix = Matrix4D::rotationZ(angleZ);
    sScene.boatTransformationMatrix = sScene.boatTransformationMatrix * Matrix4D::rotationX(angleX);

    // Input stuff

    /* if 'w' or 's' pressed, boat should move forward or backwards */
    int forward = 0;
    if (sInput.buttonPressed[0])
    {
        forward = 1;
    }
    else if (sInput.buttonPressed[1])
    {
        forward = -1;
    }

    /* if 'a' or 'd' pressed, boat should move left or right */
    int left = 0;
    if (sInput.buttonPressed[2])
    {
        left = 1;
    }
    else if (sInput.buttonPressed[3])
    {
        left = -1;
    }

    /* udpate cube transformation matrix to include new rotation if one of the keys was pressed */
    if (forward != 0 && left != 0)
    {

        float turnAmount = left * sScene.boatTurningSpeed;
        if (forward > 0)
            sScene.boatTurningRadian += turnAmount;
        else
            sScene.boatTurningRadian -= turnAmount;
    }

    sScene.boatFront = Matrix3D::rotationY(sScene.boatTurningRadian) * Vector3D(0.0f, 0.0f, 1.0f);
    sScene.boatTransformationMatrix = sScene.boatTransformationMatrix * Matrix4D::rotationY(sScene.boatTurningRadian);
    sScene.boatTranslationMatrix = sScene.boatTranslationMatrix * Matrix4D::translation(-forward * sScene.boatVelocity * dt * sScene.boatFront);

    // Camera stuff

    if (sScene.cameraMode == 2)
    {
        Vector3D newBoatPosition = sScene.boatTranslationMatrix[3];
        Vector3D boatMovement = newBoatPosition - oldBoatPosition;
        sScene.camera.position = sScene.camera.position + boatMovement;
        sScene.camera.lookAt = sScene.boatTranslationMatrix[3];
    }

    if (sInput.buttonPressed[4])
    {
        sScene.cameraMode = 1;
    }
    if (sInput.buttonPressed[5])
    {
        sScene.cameraMode = 2;
    }
}

/* function to draw all objects in the scene */
void sceneDraw()
{
    /* clear framebuffer color */
    glClearColor(135.0 / 255, 206.0 / 255, 235.0 / 255, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*------------ render scene -------------*/
    /* use shader and set the uniforms (names match the ones in the shader) */
    {
        glUseProgram(sScene.shaderColor.id);
        shaderUniform(sScene.shaderColor, "uProj", cameraProjection(sScene.camera));
        shaderUniform(sScene.shaderColor, "uView", cameraView(sScene.camera));

        /* draw water plane */
        shaderUniform(sScene.shaderColor, "uModel", sScene.waterModelMatrix);
        glBindVertexArray(sScene.water.mesh.vao);
        glDrawElements(GL_TRIANGLES, sScene.water.mesh.size_ibo, GL_UNSIGNED_INT, nullptr);

        for (int i = 0; i < NUM_CUBES; i++)
        {
            /* draw cube, requires to calculate the final model matrix from all transformations */
            shaderUniform(sScene.shaderColor, "uModel", sScene.boatTranslationMatrix * sScene.boatTransformationMatrix * sScene.cubeTranslationMatrix[i] * sScene.cubeTransformationMatrix[i] * sScene.cubeScalingMatrix[i]);
            glBindVertexArray(sScene.cubeMesh[i].vao);
            glDrawElements(GL_TRIANGLES, sScene.cubeMesh[i].size_ibo, GL_UNSIGNED_INT, nullptr);
        }
    }
    glCheckError();

    /* cleanup opengl state */
    glBindVertexArray(0);
    glUseProgram(0);
}

int main(int argc, char **argv)
{
    /* create window/context */
    int width = 1280;
    int height = 720;
    GLFWwindow *window = windowCreate("Assignment 1 - Transformations, User Input and Camera", width, height);
    if (!window)
    {
        return EXIT_FAILURE;
    }

    /* set window callbacks */
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    /*---------- init opengl stuff ------------*/
    glEnable(GL_DEPTH_TEST);

    /* setup scene */
    sceneInit(width, height);

    /*-------------- main loop ----------------*/
    double timeStamp = glfwGetTime();
    double timeStampZero = timeStamp;
    double timeStampNew = 0.0;

    /* loop until user closes window */
    while (!glfwWindowShouldClose(window))
    {
        /* poll and process input and window events */
        glfwPollEvents();

        /* update model matrix of cube */
        timeStampNew = glfwGetTime();
        sceneUpdate(timeStampNew - timeStamp, timeStampNew - timeStampZero);
        timeStamp = timeStampNew;

        /* draw all objects in the scene */
        sceneDraw();

        /* swap front and back buffer */
        glfwSwapBuffers(window);
    }

    /*-------- cleanup --------*/
    /* delete opengl shader and buffers */
    shaderDelete(sScene.shaderColor);
    waterDelete(sScene.water);
    for (int i = 0; i < NUM_CUBES; i++)
    {
        meshDelete(sScene.cubeMesh[i]);
    }

    /* cleanup glfw/glcontext */
    windowDelete(window);

    return EXIT_SUCCESS;
}
