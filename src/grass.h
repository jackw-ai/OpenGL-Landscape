#ifndef GRASS_H
#define GRASS_H
#endif

#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>

#include "texture.h"
#include "shader.h"

class Grass {
public:
    Grass();
    
    // builds VAO VBO
    void buildGrass();
    
    // generates random coordinates for grass
    static float rand_FloatRange(float a, float b);
    
    // generates lots of grass
    void generateGrass(int numGrass, float floorSize);
    
    // special loader for grass texture
    bool loadGrassTex(char const * path);
    
    // rendering, passes in shader reference for model update
    void renderGrass(Shader &grassShader);
    
    GLuint getTexID();
    
private:
    GLuint textureID;
    
    // grass vertices
    static float const grassVertices[30];
    
    // VAO and VBO
    GLuint grassVAO, grassVBO;
    
    // the grass objects
    std::vector<glm::vec3> vegetation;
};
