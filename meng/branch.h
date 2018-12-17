#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "mesh.h"

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

class Branch {
public:
    Branch();
	Branch(GLfloat init_angle, GLfloat init_y_translation, GLfloat init_length_scale=1.0f, 
            GLfloat x_pos=0.0f, GLfloat z_pos=0.0f, int strong_factor=1);
    Branch(GLfloat init_angle, GLfloat init_y_translation, Branch* init_parent, GLfloat init_length_scale=1.0f, int strong_factor=1);

    void renderBranch(GLuint uniformModel,  GLuint uniformProjection, glm::mat4x4 projection);
    //void renderBranch(GLuint uniformModel,  GLuint uniformProjection, glm::mat4x4 projection, glm::mat4x4 model);
    
	~Branch();

private:
	GLfloat angle;
    GLfloat y_translation;
    Branch* parentBranch;
    glm::mat4 model_no_scale;
    glm::mat4 model_scaled;
    GLfloat length_scale;
};