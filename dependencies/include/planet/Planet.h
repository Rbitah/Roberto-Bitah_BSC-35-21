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

    

private:
    Sphere sphere;
    
};

#endif 