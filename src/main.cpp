#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "planet/Planet.h"
#include "Timer/Timer.h"
#include <camera/camera.h>
#include <shadersPrograms/shader.h>

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f;
float lastY = 600.0f;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window, Camera &camera, float deltaTime, Planet &sun, Planet &moon)
{ // camera keys control
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // Sun's spin speed
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        sun.increaseOrbitSpeed();
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        sun.decreaseOrbitSpeed();

    // Moon's orbit speed
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        moon.increaseRotationSpeed();
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        moon.decreaseRotationSpeed();
    //closing window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Solar System", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("dependencies/include/shadersPrograms/vertexShader.glsl", "dependencies/include/shadersPrograms/fragmentShader.glsl");

    // creating sun
    Planet sun(0.2f, 72, 36, "PlanetTextureMaps/sunmap.jpg");
    sun.setPlanetPos(glm::vec3(0.0f, 0.0f, 0.0f));
    sun.setRotationSpeed(0.1f);
    sun.setScale(1.0f);

    // creating venus
    Planet venus(0.08f, 72, 36, "PlanetTextureMaps/venusmap.jpg");
    venus.setPlanetPos(glm::vec3(1.0f, 0.0f, 0.0f));
    venus.setRotationSpeed(2.0f);
    venus.setScale(1.03f);
    venus.setOrbit(0.5f, 0.4f);

    // creating earth
    Planet earth(0.09f, 72, 36, "PlanetTextureMaps/earthmap1k.jpg");
    earth.setPlanetPos(glm::vec3(1.5f, 0.0f, 0.0f));
    earth.setRotationSpeed(2.0f);
    earth.setScale(1.01f);
    earth.setOrbit(0.9f, 0.5f);

    // creating moon
    Planet moon(0.03f, 72, 36, "PlanetTextureMaps/moonmap1k.jpg");
    moon.setPlanetPos(glm::vec3(1.5f, 0.0f, 0.0f));
    moon.setRotationSpeed(2.3f);
    moon.setScale(1.01f);
    moon.setOrbit(0.14f, 4.85f);

    // creating mars
    Planet mars(0.07f, 72, 36, "PlanetTextureMaps/marsmap1k.jpg");
    mars.setPlanetPos(glm::vec3(2.0f, 0.0f, 0.0f));
    mars.setRotationSpeed(2.0f);
    mars.setScale(1.02f);
    mars.setOrbit(1.3f, 0.45f);

    // creating neptune
    Planet neptune(0.1f, 72, 36, "PlanetTextureMaps/neptunemap.jpg");
    neptune.setPlanetPos(glm::vec3(2.5f, 0.0f, 0.0f));
    neptune.setRotationSpeed(2.0f);
    neptune.setScale(1.04f);
    neptune.setOrbit(1.8f, 0.4f);

    Timer timer;
    float deltaTime = 0.0f;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        timer.start();
        processInput(window, camera, deltaTime, sun, moon);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("viewPos", camera.Position);

        // directionalLighting
        ourShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        ourShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
        ourShader.setVec3("dirLight.diffuse", glm::vec3(1.1f, 1.1f, 0.4f));
        ourShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 0.5f));

        // pointLighting
        glm::vec3 pointLightPos = sun.getPlanetPosi();
        ourShader.setVec3("pointLight.position", pointLightPos + glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setVec3("pointLight.ambient", glm::vec3(0.0f, 0.0f, 0.1f));
        ourShader.setVec3("pointLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
        ourShader.setVec3("pointLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        ourShader.setFloat("pointLight.constant", 1.0f);
        ourShader.setFloat("pointLight.linear", 0.1f);
        ourShader.setFloat("pointLight.quadratic", 0.032f);

        sun.update(deltaTime);
        // std::cout << deltaTime << std::endl;
        glm::vec3 earthPos = earth.getPlanetPosi();
        sun.draw(ourShader.ID);

        earth.update(deltaTime);
        earth.draw(ourShader.ID);
        moon.setOrbitCenter(earth.getPlanetPosi());

        moon.update(deltaTime);
        moon.draw(ourShader.ID);

        mars.update(deltaTime);
        mars.draw(ourShader.ID);

        venus.update(deltaTime);
        venus.draw(ourShader.ID);

        neptune.update(deltaTime);
        neptune.draw(ourShader.ID);

        glfwSwapBuffers(window);
        glfwPollEvents();
        timer.stop();
        deltaTime = timer.getElapsedTime();
        // std::cout << deltaTime << std::endl;
    }

    glfwTerminate();
    return 0;
}
