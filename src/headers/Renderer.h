#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
 
// Include GLEW
#include <GL/glew.h>
 
// Include GLFW
#include <GL/glfw.h>

#include "../headers/Texture.h"
#include "../headers/GameObject.h" 
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include Visitors
#include "RenderVisitor.h"
class GameObject;

class Renderer {

	private:
		std::vector<GameObject*> gameObjects;
		RenderVisitor renderVisitor;
		public:
		Renderer();
		virtual ~Renderer();

		void renderObjects(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint matrixid, GLuint modelmatrixid, GLuint viewmatrixid);
		void addObjects(GameObject& gameObject);
};
