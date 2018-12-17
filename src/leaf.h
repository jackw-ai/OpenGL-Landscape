#ifndef LEAF_H
#define LEAF_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "branch.h"
#include "tree.h"

class Leaf {
 public:
  Leaf();

  void renderLeaf(GLuint uniformModel,  GLuint uniformProjection, glm::mat4x4 projection);

  ~Leaf();
  
 private:
  GLfloat angle;
  GLfloat y_translation;
  Branch* parentBranch;
  glm::mat4 model_no_scale;
  glm::mat4 model_scaled;
  GLfloat length_scale;

}
