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
#include "shader.h"
#include "camera.h"
#include "tree.h"
#include "sky.h"

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
std::vector<glm::vec3> vegetation;
int grassCount = 2000;
static float rand_FloatRange(float a, float b);
void generateGrass(int numGrass);

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";
static const char* vSkyShader = "Shaders/skyshader.vert";
static const char* vGrShader = "Shaders/grass.vert";


// Fragment Shader
static const char* fShader = "Shaders/shader.frag";
static const char* fSkyShader = "Shaders/skyshader.frag";
static const char* fGrShader = "Shaders/grass.frag";

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
		-floorSize, 0.0f, -floorSize,	0.0f, 0.0f,		        0.0f, -1.0f, 0.0f,
		floorSize, 0.0f, -floorSize,	floorSize, 0.0f,	    0.0f, -1.0f, 0.0f,
		-floorSize, 0.0f, floorSize,	0.0f, floorSize,	    0.0f, -1.0f, 0.0f,
		floorSize, 0.0f, floorSize,		floorSize, floorSize,	0.0f, -1.0f, 0.0f
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

    // vegetation
    generateGrass(grassCount);
    /*
    vegetation.push_back(glm::vec3(-1.5f,  -1.5f, -0.48f));
    vegetation.push_back(glm::vec3( 1.5f,  -1.5f,  0.51f));
    vegetation.push_back(glm::vec3( 0.0f,  -1.5f,  0.7f));
    vegetation.push_back(glm::vec3(-0.3f,  -1.5f, -2.3f));
    vegetation.push_back(glm::vec3( 2.5f,  -2.0f, -0.8f));
    vegetation.push_back(glm::vec3( 0.5f,  -2.0f, -0.9f));
    vegetation.push_back(glm::vec3( 0.2f,  -2.0f, -0.6f));
    vegetation.push_back(glm::vec3( 0.6f,  -2.0f, -0.6f));
    vegetation.push_back(glm::vec3( 0.5f,  -2.0f, -0.6f));
    vegetation.push_back(glm::vec3( 0.5f,  -2.0f, -0.6f));
    vegetation.push_back(glm::vec3( 0.5f,  -2.0f, -0.6f));
     */
    
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
}

static float rand_FloatRange(float a, float b)
{
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

void generateGrass(int numGrass){
    
    for (int i = 0; i < numGrass; i++){
        float z = -1.5f;
        float x = rand_FloatRange(-floorSize, floorSize);
        float y = rand_FloatRange(-floorSize, floorSize);
        vegetation.push_back(glm::vec3(x,  z,  y));
    }
}

unsigned int loadGrass(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return textureID;
}

int main() 
{
	std::cout << "starting..." << std::endl;

	mainWindow = Window(1200, 800);
	mainWindow.Initialise();

	CreateObjects();
	std::cout << "objects created..." << std::endl;
    
	Tree tree1(4, 3, 1.0f, 1.0f);
	Tree tree2(2, 3, 5.0f, 8.0f);
	Tree tree3(3, 3, -7.0f, 5.0f);
    
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
	
    /*
    grassTexture = Texture("textures/grass.jpeg");
    grassTexture.LoadTexture();
     */
    
    grassTexture = loadGrass("textures/grass.png");
    
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
    
    /*
    // skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sky->getVertices()) * 108, sky->getVertices(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    */
    sky->buildSky();
    std::cout << "finished building sky..." << std::endl;

    /* vegetation */
    
    
    // grass shader
    Shader *grassShader = new Shader();
    grassShader->CreateFromFiles(vGrShader, fGrShader);
    grassShader->UseShader();
    grassShader->setInt("theTexture", 0);

    float grassVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    
    // grass VAO
    GLuint grassVAO, grassVBO;
    glGenVertexArrays(1, &grassVAO);
    glGenBuffers(1, &grassVBO);
    glBindVertexArray(grassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // store trees in vector
    std::vector<Tree> trees;

    trees.push_back(tree1);
    trees.push_back(tree2);
    trees.push_back(tree3);
    
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
        
        /*
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		tree1.keyControl(mainWindow.getsKeys());
		tree2.keyControl(mainWindow.getsKeys());
		tree3.keyControl(mainWindow.getsKeys());
        */
        
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

        // render all trees
        for (int i = 0; i < trees.size(); i++)
            trees[i].renderTree(uniformModel, uniformView, uniformProjection, projection);
       
        // vegetation
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glBindVertexArray(grassVAO);
        grassShader->UseShader();

        grassShader->setMat4("projection", projection);
        grassShader->setMat4("view", camera.calculateViewMatrix());
        for (GLuint i = 0; i < vegetation.size(); i++)
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, vegetation[i]);

            grassShader->setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
            //glBindVertexArray(0);

        }

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyShader->UseShader();
        glm::mat4 view = glm::mat4(glm::mat3(camera.calculateViewMatrix())); // remove translation from the view matrix
        skyShader->setMat4("view", view);
        skyShader->setMat4("projection", projection);
        
        sky->renderSky();

        
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

    glfwTerminate();

	return 0;
}
