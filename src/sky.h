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
    
    bool loadCubemap();
    
    GLuint getTexID();
    
    ~Sky();
    
    
private:
    GLuint textureID;
    std::vector<std::string> faces;
};
