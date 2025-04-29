#ifndef PLANET_H
#define PLANET_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Sphere/Sphere.h>
#include <string>

class Planet
{
public:
    Planet(float radius, int sectors, int stacks, const std::string &texturePath);
    ~Planet();

    void update(float deltaTime);
    void draw(unsigned int shaderProgram);

    void setPosition(const glm::vec3 &position);
    void setRotationSpeed(float speed);
    void setScale(float scale);
    void setOrbit(float radius, float speed, const glm::vec3 &center = glm::vec3(0.0f));

    void increaseRotationSpeed();
    void decreaseRotationSpeed();
    void increaseOrbitSpeed();
    void decreaseOrbitSpeed();

    float getRotationSpeed() const { return rotationSpeed; };
    float getOrbitingSpeed() const { return orbitSpeed; };
    void setOrbitCenter(const glm::vec3 &center) { orbitCenter = center; };
    glm::vec3 getPosition() const;


private:
    Sphere sphere;
    unsigned int textureID;

    glm::vec3 position;
    float rotationSpeed;
    float angle;
    float scale;

    float orbitRadius = 0.0f;
    float orbitSpeed = 0.0f;
    float orbitAngle = 0.0f;
    glm::vec3 orbitCenter = glm::vec3(0.0f);

    unsigned int loadTexture(const std::string &texPath);
};

#endif
