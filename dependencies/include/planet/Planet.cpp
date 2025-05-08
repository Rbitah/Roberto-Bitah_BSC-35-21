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
    // rotation
    angle += rotationSpeed * deltaTime;
    orbitAngle += orbitSpeed * deltaTime;
    position.x = orbitCenter.x + orbitRadius * cos(orbitAngle);
    position.z = orbitCenter.z + orbitRadius * sin(orbitAngle);
}
void Planet::draw(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = translate(trans, position);
    trans = rotate(trans, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    trans = glm::scale(trans, glm::vec3(scale));

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    sphere.draw();
}
void Planet::setPlanetPos(const glm::vec3 &pos)
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
void Planet::setOrbit(float radius, float speed, const glm::vec3 &center)
{
    orbitRadius = radius;
    orbitSpeed = speed;
    orbitCenter = center;
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
void Planet::increaseRotationSpeed()
{
    orbitSpeed += 0.1f;
}
void Planet::decreaseRotationSpeed()
{
    orbitSpeed -= 0.01f;
    if (orbitSpeed < 0.0f)
        orbitSpeed = 0.0f;
}
void Planet::increaseOrbitSpeed()
{
    rotationSpeed += 0.01f;
}
void Planet::decreaseOrbitSpeed()
{
    rotationSpeed -= 0.11f;
    if (rotationSpeed < 0.0f)
        rotationSpeed = 0.0f;
}
glm::vec3 Planet::getPlanetPosi() const
{
    return position;
}
