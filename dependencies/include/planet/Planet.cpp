#include "Planet.h"
#include <stb_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Planet::Planet(float radius, int sectors, int stacks, const std::string &texturePath)
    : sphere(radius, sectors, stacks), position(0.0f), rotationSpeed(1.0f), angle(0.0f), scale(1.0f)
{
    textureID = loadTexture(texturePath);
}

Planet::~Planet()
{
    glDeleteTextures(1, &textureID);
}

void Planet::update(float deltaTime)
{
    angle += rotationSpeed * deltaTime;
}

void Planet::draw(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale));

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    sphere.draw();
}

void Planet::setPosition(const glm::vec3 &pos)
{
    position = pos;
}

void Planet::setRotationSpeed(float speed)
{
    rotationSpeed = speed;
}

void Planet::setScale(float s)
{
    scale = s;
}

unsigned int Planet::loadTexture(const std::string &path)
{
    unsigned int texID;
    glGenTextures(1, &texID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return texID;
}
