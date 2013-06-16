#pragma once

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
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <../../src/headers/Texture.h>

class Environment {
	enum {SKY_LEFT = 0,SKY_BACK,SKY_RIGHT,SKY_FRONT,SKY_TOP,SKY_BOTTOM,PLANE};      
	unsigned int skybox[7]; //the ids for the textures

	private:
		GLuint backvertexbuffer, frontvertexbuffer, rightvertexbuffer,leftvertexbuffer,bottomvertexbuffer,topvertexbuffer, planevertexbuffer;
		GLuint uvbuffer;
		GLuint SkyboxShaders, TextureID;
	public:
		void initEnvironment();

		void drawEnvironment();
		void drawPlane();
		void drawQuad(int skyboxIndex, GLuint vertexbuffer, GLuint uvbuffer);
};