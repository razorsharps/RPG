#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// Include GLEW
#include <GL/glew.h>
 
// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif