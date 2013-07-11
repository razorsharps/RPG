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
		RenderVisitor renderVisitor;
		public:
			Renderer();
			virtual ~Renderer();
			
			std::vector<GameObject*> gameObjects;
			void renderObjects(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint matrixid);
			void addObjects(GameObject* gameObject);
};
