#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture {
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();
	
	void UseTexture();
	void ClearTexture();

    GLuint getTexID();
    
	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

