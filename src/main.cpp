#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "planet/Planet.h"
#include "Timer/Timer.h"
#include <camera/camera.h>

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f;
float lastY = 600.0f; 
bool firstMouse = true;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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
}

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aNormal;\n"
                                 "layout (location = 2) in vec2 aTexCoord;\n"
                                 "\n"
                                 "uniform mat4 transform;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "\n"
                                 "out vec2 TexCoord;\n"
                                 "\n"
                                 "void main()\n"
                                 "{\n"
                                 "    gl_Position = projection * view * transform * vec4(aPos, 1.0);\n"
                                 "    TexCoord = aTexCoord;\n"
                                 "}\n";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec2 TexCoord;\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "uniform sampler2D ourTexture;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = texture(ourTexture, TexCoord);\n"
                                   "}\n";

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
     
    //coloring


    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // creating sun
    Planet sun(0.2f, 72, 36, "PlanetTextureMaps/sunmap.jpg");
    sun.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    sun.setRotationSpeed(0.1f);
    sun.setScale(1.0f);

    // creating venus
    Planet venus(0.08f, 72, 36, "PlanetTextureMaps/venusmap.jpg");
    venus.setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    venus.setRotationSpeed(2.0f);
    venus.setScale(1.03f);
    venus.setOrbit(0.5f, 0.4f);

    // creating earth
    Planet earth(0.09f, 72, 36, "PlanetTextureMaps/earthmap1k.jpg");
    earth.setPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    earth.setRotationSpeed(2.0f);
    earth.setScale(1.01f);
    earth.setOrbit(0.9f, 0.5f);

    // creating moon
    Planet moon(0.03f, 72, 36, "PlanetTextureMaps/moonmap1k.jpg");
    moon.setPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    moon.setRotationSpeed(2.3f);
    moon.setScale(1.01f);
    moon.setOrbit(0.14f, 4.85f);
    // creating mars
    Planet mars(0.07f, 72, 36, "PlanetTextureMaps/marsmap1k.jpg");
    mars.setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    mars.setRotationSpeed(2.0f);
    mars.setScale(1.02f);
    mars.setOrbit(1.3f, 0.45f);

    // creating neptune
    Planet neptune(0.1f, 72, 36, "PlanetTextureMaps/neptunemap.jpg");
    neptune.setPosition(glm::vec3(2.5f, 0.0f, 0.0f));
    neptune.setRotationSpeed(2.0f);
    neptune.setScale(1.04f);
    neptune.setOrbit(1.8f, 0.4f);

    Timer timer;
    float deltaTime = 0.0f;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        timer.start();
        processInput(window, camera, deltaTime, sun, moon);

        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        sun.update(deltaTime);
        std::cout << deltaTime << std::endl;
        glm::vec3 earthPos = earth.getPosition();
        sun.draw(shaderProgram);

        earth.update(deltaTime);
        earth.draw(shaderProgram);
        moon.setOrbitCenter(earth.getPosition());

        moon.update(deltaTime);
        moon.draw(shaderProgram);

        mars.update(deltaTime);
        mars.draw(shaderProgram);

        venus.update(deltaTime);
        venus.draw(shaderProgram);

        neptune.update(deltaTime);
        neptune.draw(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
        timer.stop();
        deltaTime = timer.getElapsedTime();
        std::cout << deltaTime << std::endl;
    }

    glfwTerminate();
    return 0;
}
