#ifndef SKY_H
#define SKY_H
#endif

#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>

#include "texture.h"

class Sky {
public:
    Sky();
    
    Sky(float skyboxSize);

    bool loadCubemap();
    
    void buildSky();
    
    void renderSky();
    
    GLuint getTexID();
    
    GLfloat* getVertices();
    
    ~Sky();
    
    
private:
    GLuint textureID;
    
    // filepaths to 6 faces
    std::vector<std::string> faces;
    
    // skybox vertices
    std::vector<GLfloat> skyboxVertices;
    
    // VAO and VBO
    GLuint skyboxVAO, skyboxVBO;
};
