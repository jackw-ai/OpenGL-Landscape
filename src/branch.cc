#include "branch.h"
#include <cstdlib>
#include <iostream>

Branch::Branch() {
  leaf = false;
  angle = 0.0f;
  y_translation = 0.0f;
  parentBranch = NULL;
  model_no_scale = glm::mat4(1.0);
  model_scaled = glm::mat4(1.0);
  length_scale = 1.0f;
}

Branch::Branch(GLfloat init_angle, GLfloat init_y_translation, GLfloat init_length_scale, 
                GLfloat x_pos, GLfloat z_pos, int strong_factor, bool init_leaf) {
  leaf = init_leaf;
  angle = init_angle;
  y_translation = init_y_translation;
  parentBranch = NULL;
  length_scale = init_length_scale;
  
  model_no_scale = glm::mat4(1.0);
  model_no_scale = glm::translate(model_no_scale, glm::vec3(x_pos, 0.0f, z_pos));
  model_no_scale = glm::translate(model_no_scale, glm::vec3(glm::sin(glm::radians(angle))*length_scale, y_translation, -2.5f));
  model_scaled = glm::scale(model_no_scale, glm::vec3(0.4f*(strong_factor*0.7), length_scale, 0.4f*(strong_factor*0.7)));
}

Branch::Branch(GLfloat init_angle, GLfloat init_y_translation, Branch* init_parent, 
                GLfloat init_length_scale, int strong_factor, bool init_leaf) {
  leaf = init_leaf;
  angle = init_angle;
  y_translation = init_y_translation;
  length_scale = init_length_scale;
  parentBranch = init_parent;

  GLfloat z_angle = 30.0f;

  model_no_scale = parentBranch->model_no_scale;

  model_no_scale = glm::translate(model_no_scale, glm::vec3(glm::sin(glm::radians(angle))*length_scale, y_translation, 0.0f));

  glm::vec3 rotationAxis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), 
				      glm::normalize(glm::vec3(glm::sin(glm::radians(angle)), 
							       glm::cos(glm::radians(angle)), 0.0f)));															 
  rotationAxis = glm::normalize(rotationAxis);
  if (angle > 0) {
    model_no_scale = glm::rotate(model_no_scale, glm::radians(angle), rotationAxis);
  }
  else {
    model_no_scale = glm::rotate(model_no_scale, -glm::radians(angle), rotationAxis);
  }
  if(!leaf) {
    model_no_scale = glm::rotate(model_no_scale, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  else {
    model_no_scale = glm::rotate(model_no_scale, glm::radians(0.0f+rand()%100), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  model_scaled = glm::scale(model_no_scale, glm::vec3(0.2f*(strong_factor*0.7), length_scale, 0.2f*(strong_factor*0.7)));
}

void Branch::renderBranch(GLuint uniformModel,  GLuint uniformProjection, glm::mat4x4 projection) {  
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model_scaled));
  glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
}

Branch::~Branch() {
  delete parentBranch;
}
