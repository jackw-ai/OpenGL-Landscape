#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "window.h"
#include "camera.h"
#include "tree.h"
#include "sky.h"
#include "grass.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Mesh*> treeMeshList;
std::vector<Shader> shaderList;

Camera camera;

Texture floorTexture;
Texture branchTexture;
Texture leafTexture;
GLuint grassTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;
bool rotate = false;
float rotateSpeed = 0.02f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// size of skybox
float skyboxSize = 50.0f;
float floorSize = 30.0f;

// vegetation
int grassCount = 10000;

Grass grass;
/*
 
 std::vector<glm::vec3> vegetation;

 static float rand_FloatRange(float a, float b);
void generateGrass(int numGrass);
*/

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";
static const char* vSkyShader = "Shaders/skyshader.vert";
static const char* vGrShader = "Shaders/grass.vert";


// Fragment Shader
static const char* fShader = "Shaders/shader.frag";
static const char* fSkyShader = "Shaders/skyshader.frag";
static const char* fGrShader = "Shaders/grass.frag";

void CreateObjects() {
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-floorSize, 0.0f, -floorSize,	0.0f, 0.0f,		        0.0f, -1.0f, 0.0f,
		floorSize, 0.0f, -floorSize,	floorSize, 0.0f,	    0.0f, -1.0f, 0.0f,
		-floorSize, 0.0f, floorSize,	0.0f, floorSize,	    0.0f, -1.0f, 0.0f,
		floorSize, 0.0f, floorSize,		floorSize, floorSize,	0.0f, -1.0f, 0.0f
	};

    // vegetation
    grass = Grass();
    grass.generateGrass(grassCount, floorSize);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders() {
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

// wrapper for keyboard functions across all classes
void keyboard(bool* keys, GLfloat deltaTime, Camera& camera, std::vector<Tree>& trees){
    
    camera.keyControl(keys, deltaTime);
    
    for (int i = 0; i < trees.size(); i++){
        trees[i].keyControl(keys);
    }
    
    // whether to rotate
    if (keys[GLFW_KEY_R]) {
        rotate = !rotate;
        
        if (rotate)
            std::cout << "Rotation turned on" << std::endl;
        else
            std::cout << "Rotation turned off" << std::endl;
    }
    
    if (keys[GLFW_KEY_RIGHT]){
        rotateSpeed += 0.01f;
        std::cout << "Rotation speed increased to " << rotateSpeed <<std::endl;
    }
    
    if (keys[GLFW_KEY_LEFT]){
        rotateSpeed -= 0.01f;
        std::cout << "Rotation speed decreased to " << rotateSpeed <<std::endl;

    }

    // add more trees
    // WARNING may slow things down
	if (keys[GLFW_KEY_M]){
        Tree tree2(2, 3, &branchTexture, &leafTexture, 5.0f, 8.0f);
		Tree tree3(3, 3, &branchTexture, &leafTexture, -7.0f, 5.0f, 3);

		trees.push_back(tree2);
    	trees.push_back(tree3);
    }

	
}

void loadTexture() {
	floorTexture = Texture("textures/grass.jpeg");
	floorTexture.LoadTexture();

	branchTexture = Texture("textures/cortex.bmp");
	branchTexture.LoadTexture();
	leafTexture = Texture("textures/leaf.jpeg");
	leafTexture.LoadTexture();

	grass.loadGrassTex("textures/grass.png");
    grassTexture = grass.getTexID();
}

int main() 
{
	std::cout << "starting..." << std::endl;

	mainWindow = Window(1200, 800);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 10.0f, 5.0f, 0.02f);

	std::cout << "camera finished..." << std::endl;

	loadTexture();
	std::cout << "finished loading texture..." << std::endl;

    /* skybox */
    
    // skybox textures
    Sky *sky = new Sky(skyboxSize);
    sky->loadCubemap();
    GLuint cubemapTexture = sky->getTexID();

    // skybox shader
    Shader *skyShader = new Shader();
    skyShader->CreateFromFiles(vSkyShader, fSkyShader);
    skyShader->UseShader();
    skyShader->setInt("skybox", 0);
    
    sky->buildSky();
    std::cout << "finished building sky..." << std::endl;

    /* vegetation */
    
    grass.buildGrass();

    // grass shader
    Shader *grassShader = new Shader();
    grassShader->CreateFromFiles(vGrShader, fGrShader);
    grassShader->UseShader();
    grassShader->setInt("theTexture", 0);
    
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);	
	
    // store trees in vector
    std::vector<Tree> trees;
	Tree tree1(4, 3, &branchTexture, &leafTexture, 1.0f, 1.0f);
    trees.push_back(tree1);

	std::cout << "objects created..." << std::endl;
    std::cout << "entering loop" << std::endl;

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

        // mouse controls
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        keyboard(mainWindow.getsKeys(), deltaTime, camera, trees);
        
		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}
        
        // controlled rotation speed and whether to rotate
        if (rotate){
            curAngle += rotateSpeed;
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
        }
        
		// Clear the window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()*CreateRotateY(glm::radians(curAngle))));

        // render all trees
        for (int i = 0; i < trees.size(); i++)
            trees[i].renderTree(uniformModel, uniformView, uniformProjection, projection);
       
        // vegetation
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        grassShader->UseShader();
    
        grassShader->setMat4("projection", projection);
        grassShader->setMat4("view", camera.calculateViewMatrix()*CreateRotateY(glm::radians(curAngle)));
        
        grass.renderGrass(*grassShader);

		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorTexture.UseTexture();
		meshList[0]->RenderMesh();

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyShader->UseShader();
        glm::mat4 view = glm::mat4(glm::mat3(camera.calculateViewMatrix()*CreateRotateY(glm::radians(curAngle)))); // remove translation from the view matrix
        skyShader->setMat4("view", view);
        skyShader->setMat4("projection", projection);
        
        sky->renderSky();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

    glfwTerminate();

	return 0;
}
