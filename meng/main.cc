#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "window.h"
#include "shader.h"
#include "camera.h"
#include "tree.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Mesh*> treeMeshList;
std::vector<Shader> shaderList;
Camera camera;

Texture obj1Texture;
Texture obj2Texture;
Texture floorTexture;
Texture branchTexture1;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() {
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	int segments = 10;
	int num_cylinder_vertices = 4 * 10;
	GLfloat cylinderVertices[40*8] = {};

	GLfloat const bottom = 0.0f;
	GLfloat const top    = 1.0f;
	int num= 0;
	GLfloat r = 0.2f;
	for(GLuint n = 0; n < segments; ++n) {
		GLfloat const t0 = 2 * M_PI * (float)n / (float)segments;
		GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segments;
		//quad vertex 0
		cylinderVertices[num++] = sin(t0) * r;  // x
		cylinderVertices[num++] = bottom;			// y
		cylinderVertices[num++] = cos(t0) * r;	// z
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		//quad vertex 1
		cylinderVertices[num++] = sin(t1) * r;
		cylinderVertices[num++] = bottom;
		cylinderVertices[num++] = cos(t1) * r;
		cylinderVertices[num++] = 0.5f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		//quad vertex 2
		cylinderVertices[num++] = sin(t1) * r;
		cylinderVertices[num++] = top;
		cylinderVertices[num++] = cos(t1) * r;
		cylinderVertices[num++] = 0.5f;
		cylinderVertices[num++] = 1.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		//quad vertex 3
		cylinderVertices[num++] = sin(t0) * r;
		cylinderVertices[num++] = top;
		cylinderVertices[num++] = cos(t0) * r;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.5f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
	}

	num = 0;
	int num_indices = 6*10;
	unsigned int cylinderIndices[6*10] = {};
	for(GLuint n = 0; n < segments; ++n) {
		//quad vertex 0
		cylinderIndices[num++] = n*4+0;  
		cylinderIndices[num++] = n*4+1;			
		cylinderIndices[num++] = n*4+2;	

		cylinderIndices[num++] = n*4+0;  
		cylinderIndices[num++] = n*4+2;		
		cylinderIndices[num++] = n*4+3;	
	}

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh *cylinder = new Mesh();
	cylinder->CreateMesh(cylinderVertices, cylinderIndices, num_cylinder_vertices*8, num_indices);
	meshList.push_back(cylinder);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

glm::mat4 CreateRotateY(float angle) {
    return glm::mat4(cosf(angle), 0.f, -sinf(angle), 0.f,
                    0.f,         1.f, 0.f,          0.f,
                    sinf(angle), 0.f, cosf(angle),  0.f,
                    0.f,         0.f, 0.f,          1.f);
}

int main() 
{
	std::cout << "starting..." << std::endl;

	mainWindow = Window(1200, 800);
	mainWindow.Initialise();

	CreateObjects();
	std::cout << "objects created..." << std::endl;
	Tree tree(3, 3);
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 10.0f, 5.0f, 0.02f);

	std::cout << "camera finished..." << std::endl;

	obj1Texture = Texture("textures/blurry-bright.jpg");
	obj1Texture.LoadTexture();
	obj2Texture = Texture("textures/brick.png");
	obj2Texture.LoadTextureA();
	floorTexture = Texture("textures/grass.jpeg");
	floorTexture.LoadTexture();
	std::cout << "first textures finished..." << std::endl;

	branchTexture1 = Texture("textures/cortex.bmp");
	branchTexture1.LoadTexture();
	
	std::cout << "finished loading texture..." << std::endl;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	std::cout << "entering loop" << std::endl;

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		tree.keyControl(mainWindow.getsKeys());

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		curAngle += 0.3f;
		if (curAngle >= 360) {
			curAngle -= 360;
		}

		if (direction) {
			curSize += 0.01f;
		}
		else {
			curSize -= 0.01f;
		}

		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}


		// Clear the window
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(triOffset-2, 1.0f, 2.5f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		obj1Texture.UseTexture();
		meshList[0]->RenderMesh();

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()*CreateRotateY(glm::radians(curAngle))));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-triOffset-4, 1.0f, 2.5f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		obj2Texture.UseTexture();
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorTexture.UseTexture();
		//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -2.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		branchTexture1.UseTexture();
		meshList[3]->RenderMesh();

		tree.renderTree(uniformModel, uniformView, uniformProjection, projection);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}