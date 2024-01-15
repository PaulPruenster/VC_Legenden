#include <cstdlib>
#include <iostream>

#include "mygl/shader.h"
#include "mygl/mesh.h"
#include "mygl/camera.h"

#include <glm/gtc/matrix_transform.hpp>

struct
{
    Camera camera;
    std::vector<Mesh> meshes;
    int rendermode = 0;

    ShaderProgram shaderColor;
} sScene;

struct
{
    bool mouseButtonPressed = false;
    glm::vec2 mousePressStart;
} sInput;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
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

    /* switch render mode (polygons, wireframe, vertex points) */
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        sScene.rendermode = (sScene.rendermode + 1) % 3;

        if (sScene.rendermode == 0)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if (sScene.rendermode == 1)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
    }
}

void mouse_pos_callback(GLFWwindow *window, double x, double y)
{
    /* called on cursor position change */
    if (sInput.mouseButtonPressed)
    {
        glm::vec2 diff = sInput.mousePressStart - glm::vec2(x, y);
        cameraUpdateOrbit(sScene.camera, diff, 0.0f);
        sInput.mousePressStart = glm::vec2(x, y);
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    /* called on mouse button event */
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        sInput.mouseButtonPressed = (action == GLFW_PRESS);

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        sInput.mousePressStart = glm::vec2(x, y);
    }
}

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    cameraUpdateOrbit(sScene.camera, {0, 0}, yoffset * 0.1);
}

void window_resize_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    sScene.camera.width = width;
    sScene.camera.height = height;
}

int main(int argc, char **argv)
{
    /* create window/context */
    GLFWwindow *window = windowCreate("tinyobjloader Demo", 1280, 720);

    /* set window callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetFramebufferSizeCallback(window, window_resize_callback);

    /*---------- init opengl stuff ------------*/
    glEnable(GL_DEPTH_TEST);

    /* load obj file and create opengl buffers for meshes */
    sScene.meshes = meshLoadFromObj("assets/Lowpoly_City_Free_Pack.obj");

    /* load shader from file */
    sScene.shaderColor = shaderLoad("shader/default.vert", "shader/color.frag");

    /* create camera */
    sScene.camera = cameraCreate(1280, 720, glm::radians(45.0), 0.01, 100.0, {0, 5, -10.0});

    /*-------------- main loop ----------------*/
    float t = 0.0f;
    /* loop until user closes window */
    while (!glfwWindowShouldClose(window))
    {
        /* poll and process input and window events */
        glfwPollEvents();
        t += 1.0 / 60.0f;

        /*------------ default frambuffer -------------*/
        {
            glClearColor(1.0, 1.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /*------------ render cube (color) -------------*/
            /* use shader and set the uniforms (names match the ones in the shader) */
            {
                /* setup camera and model matrices */
                glm::mat4 proj = cameraProjection(sScene.camera);
                glm::mat4 view = cameraView(sScene.camera);
                glm::mat4 model = glm::mat4(1.0f);
                // model = glm::rotate(model, (float)M_PI / 8.0f * t, glm::vec3(0.0f, 1.0f, 0.0f));

                glUseProgram(sScene.shaderColor.id);
                shaderUniform(sScene.shaderColor, "uProj", proj);
                shaderUniform(sScene.shaderColor, "uView", view);
                shaderUniform(sScene.shaderColor, "uModel", model);

                /* draw all meshes loaded from obj file */
                for (unsigned int m = 0; m < sScene.meshes.size(); m++)
                {
                    /* bind vertex array object and draw its content */
                    glBindVertexArray(sScene.meshes[m].vao);
                    glDrawElements(GL_TRIANGLES, sScene.meshes[m].size_ibo, GL_UNSIGNED_INT, nullptr);
                }
            }

            /* cleanup opengl state */
            glBindVertexArray(0);
            glUseProgram(0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /* swap front and back buffer */
        glfwSwapBuffers(window);
    }

    /*-------- cleanup --------*/
    /* delete opengl shader and buffers */
    shaderDelete(sScene.shaderColor);
    for (unsigned int m = 0; m < sScene.meshes.size(); m++)
    {
        meshDelete(sScene.meshes[m]);
    }

    /* destroy window/context */
    windowDelete(window);

    return EXIT_SUCCESS;
}
