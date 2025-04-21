#ifndef PLANET_H
#define PLANET_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Sphere/Sphere.h>
#include <string>

class Planet {
public:
    Planet(float radius, int sectors, int stacks, const std::string& texturePath);
    ~Planet();

    void update(float deltaTime);
    void draw(unsigned int shaderProgram);

    void setPosition(const glm::vec3& position);
    void setRotationSpeed(float speed);
    void setScale(float scale);

private:
    Sphere sphere;
    unsigned int textureID;

    glm::vec3 position;
    float rotationSpeed;
    float angle;
    float scale;

    unsigned int loadTexture(const std::string& texPath);
};

#endif
