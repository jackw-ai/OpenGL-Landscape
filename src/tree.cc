#include "tree.h"
#include <iostream>

Tree::Tree() {
}

Tree::Tree(int init_depth, int init_height, GLfloat init_x_pos, GLfloat init_z_pos, int init_strong_factor) {
	branchTexture = Texture("textures/cortex.bmp");
	branchTexture.LoadTexture();

	max_depth = init_depth;
	max_height = init_height;
	x_pos = init_x_pos;
	z_pos = init_z_pos;
	strong_factor = init_strong_factor;

	// cortex
	Branch* cortex = new Branch(0.0f, -1.0f, 2.0f, x_pos, z_pos, strong_factor+(max_depth/5));
	branchList.push_back(cortex);

	// root branch
	Branch* root_branch = new Branch(0.0f, 2.0f, 1.0f, x_pos, z_pos, strong_factor+(max_depth/5));
	branchList.push_back(root_branch);


	// // build the tree per level of height given depth
	Branch* level_root_branch = root_branch;
	for (int h=1; h <= max_height; h++) {
		buildTree(level_root_branch, max_depth, h, max_depth);

		if (h < max_height) {
			level_root_branch = new Branch(0.0f, 2.0f+h*2.0f, 1.0f, x_pos, z_pos, strong_factor+(max_depth/5));
			branchList.push_back(level_root_branch);
		}
	}
}

void Tree::buildTree(Branch* root_branch, int tree_depth, int tree_height, int total_depth) {
	if (tree_depth == 0) {
		return;
	}

	int child_number = 2;
	for (int i=0; i < child_number; i++) {
		GLfloat angle = 60.0f * (1+tree_height) * 0.2;
		GLfloat y_translation = 1.0f*0.4;
		if (tree_height == 1 && tree_depth == total_depth) {
			y_translation *= 2;
		}
		if (i == 1) {
			angle *= -1;
			y_translation *= 2;
		}
		Branch* branch = new Branch(angle, y_translation, root_branch, 1.0f, strong_factor+(tree_depth/5));
		branchList.push_back(branch);

		buildTree(branch, tree_depth-1, tree_height, total_depth);

	}
}

void Tree::CreateCubeBranchMesh() {
	// initialize a cube
	int indicesLen = 36;  // 3 * 12
	int verticesLen = 64; // 8 * 8

	// 12 triangles
	unsigned int indices[] = {
		0, 1, 2,
		5, 0, 3,
		6, 0, 7,
		5, 7, 0,
		0, 2, 3, 
		6, 1, 0,
		2, 1, 6,
		4, 7, 5,
		7, 4, 6, 
		4, 5, 3,
		4, 3, 2, 
		4, 2, 6
	};

	// 8 vertices
	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -1.0f,       0.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //0
		-1.0f, -1.0f, 1.0f,        0.5f, 0.0f,     0.0f, -0.0f, 0.0f,  //1
		-1.0f, 1.0f, 1.0f,         1.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //2
		-1.0f, 1.0f, -1.0f,        0.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //3
		1.0f, 1.0f, 1.0f,          0.0f, 1.0f,     0.0f, -0.0f, 0.0f,  //4
		1.0f, 1.0f, -1.0f,         1.0f, 1.0f,     0.0f, -0.0f, 0.0f,  //5
		1.0f, -1.0f, 1.0f,         0.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //6
		1.0f, -1.0f, -1.0f,        0.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //7
	};

	Mesh* cube = new Mesh();
	cube->CreateMesh(vertices, indices, verticesLen, indicesLen);
	treeMeshList.push_back(cube);
}

void Tree::CreateCylinderBranchMesh() {
	int segments = 10;
	int num_cylinder_vertices = 4 * 10;
	GLfloat cylinderVertices[40*8] = {};

	GLfloat const bottom = -1.0f;
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

	Mesh* cylinder = new Mesh();
	cylinder->CreateMesh(cylinderVertices, cylinderIndices, num_cylinder_vertices*8, num_indices);
	treeMeshList.push_back(cylinder);
}

void Tree::renderTree(GLuint uniformModel, GLuint uniformView, GLuint uniformProjection, glm::mat4x4 projection) {
	//for (int i = 0; i < branchList.size(); i++) {
	for (int i = 0; i < numBranchRendered; i++) {
		CreateCylinderBranchMesh();
		Branch* curr_branch = branchList[i];

		curr_branch->renderBranch(uniformModel, uniformProjection, projection);
		branchTexture.UseTexture();
		treeMeshList[i]->RenderMesh();
	}
}

void Tree::keyControl(bool* keys) {
    
	if (keys[GLFW_KEY_G]) {
		growUpward(keys);
	}

	if (keys[GLFW_KEY_B]) {
		growDownward(keys);
	}
}

void Tree::growUpward(bool* keys) {
	numBranchRendered += 2;

	if (numBranchRendered > (int)branchList.size()) {
		numBranchRendered = (int)branchList.size();
	}
	else if (numBranchRendered < 0) {
		numBranchRendered = 0;
	}


}

void Tree::growDownward(bool* keys) {
	numBranchRendered -= 2;

	if (numBranchRendered > (int)branchList.size()) {
		numBranchRendered = (int)branchList.size();
	}
	else if (numBranchRendered < 0) {
		numBranchRendered = 0;
	}
}

Tree::~Tree() {
}

