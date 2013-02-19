// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
 
// Include GLEW
#include <GL/glew.h>
 
// Include GLFW
#include <GL/glfw.h>
 
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#ifndef FUNCTION_UTILS_H
#define FUNCTION_UTILS_H

void initSkybox(float size);
void drawSkybox();
void drawQuad(int skyboxIndex, GLuint vertexbuffer, GLuint uvbuffer);

#endif // QUATERNION_UTILS_H